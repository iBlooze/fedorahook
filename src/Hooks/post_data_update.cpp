#include "hooks.hpp"
#include "../Utilities/item_definitions.hpp"
#include "../main.hpp"
#include "../Utilities/config.hpp"
#include "../Utilities/kit_parser.hpp"
#include "../SDK/VMatrix.hpp"

static auto get_wearable_create_fn() -> sdk::CreateClientClassFn
{
	auto clazz = g_client->GetAllClasses();

	// change to classid...
	while (fnv::hash_runtime(clazz->m_pNetworkName) != FNV("CEconWearable"))
		clazz = clazz->m_pNext;

	return clazz->m_pCreateFn;
}

static auto make_glove(int entry, int serial) -> sdk::C_BaseAttributableItem*
{
	static auto create_wearable_fn = get_wearable_create_fn();

	create_wearable_fn(entry, serial);

	const auto glove = static_cast<sdk::C_BaseAttributableItem*>(g_entity_list->GetClientEntity(entry));
	static auto set_abs_origin_addr = platform::find_pattern(get_client_name(), "\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1", "xxxxxxxxxxxx");
	const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(set_abs_origin_addr);
	static const Vector new_pos = { 10000.f, 10000.f, 10000.f };

	assert(glove);
	set_abs_origin_fn(glove, new_pos);

	return glove;
}

static auto apply_config_on_attributable_item(sdk::C_BaseAttributableItem* weapon, const unsigned xuid_low, sdk::C_BasePlayer* local, bool glove) -> void
{
	auto class_id = weapon->GetClientClass()->m_ClassID,
		definition_index = (int)weapon->GetItemDefinitionIndex(),
		model_index = game_data::model_names[glove ? game_data::glove_model : game_data::knife_model + 9].definition_index;

	auto config = game_data::w_settings[definition_index];
	auto replacement_item = game_data::get_weapon_info(model_index);
	if (!replacement_item)
		return;

	// default values / fix stattrack
	weapon->GetItemIDHigh() = -1;
	weapon->GetAccountID() = xuid_low;

	// change the name
	if (config.name[0])
		strcpy_s(weapon->GetCustomName(), config.name);

	// edit stattrack values
	if (config.stat_track)
		weapon->GetFallbackStatTrak() = config.stat_track;

	// change the skin / seed / wear
	weapon->GetFallbackSeed() = config.seed;
	weapon->GetFallbackWear() = config.wear;
	weapon->GetFallbackPaintKit() = config.skin;

	config = game_data::w_settings[model_index];

	// update models (knives / gloves)`
	if ((class_id == 107 || class_id == 108) && model_index != definition_index || glove && model_index != definition_index)
	{
		// change definition index
		weapon->GetItemDefinitionIndex() = short(model_index);

		// recall this here (needs to be called after GetItemDefinitionIndex
		weapon->GetFallbackSeed() = game_data::w_settings[model_index].seed;
		weapon->GetFallbackWear() = game_data::w_settings[model_index].wear;
		weapon->GetFallbackPaintKit() = game_data::w_settings[model_index].skin;

		// change knife model
		weapon->SetModelIndex(g_model_info->GetModelIndex(replacement_item->model));

		// update data
		weapon->GetClientNetworkable()->PreDataUpdate(0);
	}
}

static auto post_data_update_start(sdk::C_BasePlayer* local) -> void
{
	sdk::player_info_t player_info;
	const auto local_index = local->GetIndex();
	if (!g_engine->GetPlayerInfo(local_index, &player_info))
		return;

	// Handle glove config
	{
		const auto wearables = local->GetWearables();
		static auto glove_handle = sdk::CBaseHandle(0);
		auto glove = get_entity_from_handle<sdk::C_BaseAttributableItem>(wearables[0]);

		// There is no glove
		if (!glove)
		{
			// Try to get our last created glove
			const auto our_glove = get_entity_from_handle<sdk::C_BaseAttributableItem>(glove_handle);

			// Our glove still exists
			if (our_glove)
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}

		if (local->GetLifeState() != sdk::LifeState::ALIVE)
		{
			// We are dead but we have a glove, destroy it
			if (glove)
			{
				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
				glove->GetClientNetworkable()->Release();
			}

			return;
		}

		// We don't have a glove, but we should
		if (game_data::glove_model && !glove)
		{
			// We don't have a glove, but we should
			if (!glove)
			{
				const auto entry = g_entity_list->GetHighestEntityIndex() + 1;
				const auto serial = rand() % 0x1000;

				glove = make_glove(entry, serial);

				wearables[0] = entry | serial << 16;

				// Let's store it in case we somehow lose it.
				glove_handle = wearables[0];
			}

			// Thanks, Beakers
			glove->GetIndex() = -1;
			apply_config_on_attributable_item(glove, player_info.xuid_low, local, true);
		}
	}

	// Handle weapon configs
	for (auto weapon_handle : local->GetWeapons())
	{
		if (weapon_handle == sdk::INVALID_EHANDLE_INDEX)
			break;

		auto weapon = get_entity_from_handle<sdk::C_BaseAttributableItem>(weapon_handle);
		if (!weapon)
			continue;

		apply_config_on_attributable_item(weapon, player_info.xuid_low, local, false);
	}

	const auto view_model = get_entity_from_handle<sdk::C_BaseViewModel>(local->GetViewModel());
	if (!view_model)
		return;

	const auto view_model_weapon = get_entity_from_handle<sdk::C_BaseAttributableItem>(view_model->GetWeapon());
	if (!view_model_weapon)
		return;

	const auto override_info = game_data::get_weapon_info(view_model_weapon->GetItemDefinitionIndex());
	if (!override_info)
		return;

	const auto world_model = get_entity_from_handle<sdk::CBaseWeaponWorldModel>(view_model_weapon->GetWeaponWorldModel());
	if (!world_model)
		return;

	view_model->GetModelIndex() = g_model_info->GetModelIndex(override_info->model);
	world_model->GetModelIndex() = g_model_info->GetModelIndex(override_info->model) + 1;
}

bool hooks::valid_entity(sdk::C_BasePlayer* ent)
{
	if (ent == g_local
		|| ent->IsDormant()
		|| ent->GetLifeState() != sdk::LifeState::ALIVE
		|| ent->GetTeamNumber() == g_local->GetTeamNumber())
		return false;

	else return true;
}

int hooks::parse_skins()
{
	valve_parser::Document doc;
	auto r = doc.Load(".\\csgo\\scripts\\items\\items_game.txt", valve_parser::ENCODING::UTF8);
	if (!r)
		return 1;

	valve_parser::Document english;
	r = english.Load(".\\csgo\\resource\\csgo_english.txt", valve_parser::ENCODING::UTF16_LE);
	if (!r)
		return 2;

	auto weapon_skins = doc.BreadthFirstSearch("weapon_icons");
	if (!weapon_skins || !weapon_skins->ToObject())
		return 3;

	auto skin_data_vec = doc.BreadthFirstSearchMultiple("paint_kits");
	if (!skin_data_vec.size())
		return 4;

	auto paint_kits = english.BreadthFirstSearch("Tokens");
	if (!paint_kits || !paint_kits->ToObject())
		return 5;

	std::vector<std::string> weaponNames =
	{
		// weapons
		"deagle", "elite", "fiveseven","glock","ak47","aug","awp","famas","g3sg1","galilar","m249","m4a1_silencer","m4a1","mac10","p90","ump45","xm1014","bizon",
		"mag7","negev","sawedoff","tec9","hkp2000","mp7","mp9","nova","p250","scar20","sg556","ssg08","usp_silencer","cz75a","revolver",

		// knives
		"knife_m9_bayonet",	"bayonet","knife_flip","knife_gut","knife_karambit","knife_tactical","knife_falchion","knife_survival_bowie","knife_butterfly","knife_push", "knife_cord", "knife_canis", "knife_ursus", "knife_gypsy_jackknife", "knife_outdoor", "knife_stiletto", "knife_widowmaker", "knife_skeleton",

		// gloves
		"studded_bloodhound_gloves","sporty_gloves","slick_gloves","leather_handwraps","motorcycle_gloves","specialist_gloves","studded_hydra_gloves"
	};

	//populate G::weaponSkins
	for (auto child : weapon_skins->children)
	{
		if (!child->ToObject())
			continue;

		for (auto weapon : weaponNames)
		{
			auto name = child->ToObject()->getKeyByName("icon_path")->ToKeyValue()->Value.toString();
			auto pos = name.find(weapon);

			//filter out the skinname
			if (pos != std::string::npos)
			{
				auto pos2 = name.find_last_of('_');
				game_data::weapon_skins[weapon].insert(name.substr(pos + weapon.length() + 1, pos2 - pos - weapon.length() - 1));
				break;
			}
		}
	}

	//populate skinData
	for (auto skinData : skin_data_vec)
	{
		if (!skinData->ToObject())
			continue;

		for (auto skin : skinData->children)
		{
			if (!skin->ToObject())
				continue;

			game_data::paint_kit si;
			si.paintkit = skin->ToObject()->name.toInt();

			auto skinName = skin->ToObject()->getKeyByName("name")->ToKeyValue()->Value.toString();
			auto tagNode = skin->ToObject()->getKeyByName("description_tag");
			if (tagNode)
			{
				std::string tag = tagNode->ToKeyValue()->Value.toString();
				tag = tag.substr(1, std::string::npos); //skip #
				std::transform(tag.begin(), tag.end(), tag.begin(), towlower);
				si.name = tag;
			}

			auto keyVal = skin->ToObject()->getKeyByName("seed");
			if (keyVal != nullptr)
				si.seed = keyVal->ToKeyValue()->Value.toInt();

			game_data::skin_map[skinName] = si;
		}
	}

	//populate G::skinNames
	for (auto child : paint_kits->children)
	{
		if (!child->ToKeyValue())
			continue;

		std::string key = child->ToKeyValue()->Key.toString();
		std::transform(key.begin(), key.end(), key.begin(), towlower);
		if (key.find("paintkit") != std::string::npos && key.find("tag") != std::string::npos)
			game_data::skin_names[key] = child->ToKeyValue()->Value.toString();
	}

	return 0;
}

auto __fastcall hooks::CCSPlayer_PostDataUpdate::hooked(sdk::IClientNetworkable* thisptr, void*, int update_type) -> void
{
	g_local = static_cast<sdk::C_BasePlayer*>(thisptr);
	post_data_update_start(static_cast<sdk::C_BasePlayer*>(thisptr));

	return m_original(thisptr, nullptr, update_type);
}

hooks::CCSPlayer_PostDataUpdate::Fn* hooks::CCSPlayer_PostDataUpdate::m_original;