#include <imgui.h>
#include <functional>
#include "sdk.hpp"
#include "Utilities/config.hpp"
#include "Utilities/update_check.hpp"
#include "Utilities/item_definitions.hpp"
#include "Utilities/render.hpp"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>

#define UNLEN 256
using namespace game_data;
bool compare_alphabet(const weapon_name& a, const weapon_name& b)
{
	if (a.name != b.name)
		return a.name < b.name;
	else
		return a.definition_index < b.definition_index;
}

bool tab(const char* label, const ImVec2& size_arg, int* page, int in, ImColor clr, bool border) {
	bool r = ImGui::button_tab(label, size_arg, 0, *page, in, border, clr);
	ImGui::SameLine(0, 0);
	if (r) *page = in;
	return r;
}

enum pages {
	PAGE_LEGITBOT = 0,
	PAGE_MISC,
	PAGE_SKINS,
	PAGE_MODELS,
	PAGE_CONFIG
};

void fix_child()
{
	ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
	ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
}

void settings_tab()
{
	bool b;
	auto cur_pos = ImGui::GetCursorScreenPos();
	const char* weapons[] = { "pistol" , "automatic weapon" , "sniper / shotgun" };
	std::string tab_name = weapons[legit_weapon_setting];
	tab_name += " settings";

	// 1st column
	ImGui::Columns(2, nullptr, false);

	fix_child();
	ImGui::BeginChild("aimbot", ImVec2(229, 442), true);
	{
		ImGui::Checkbox("enable", &legit_enable);

		ImGui::Combo("selected weapon", &legit_weapon_setting, weapons, ARRAYSIZE(weapons));

		ImGui::Checkbox("enable rcs", &legit_rcs_enable);
		ImGui::Checkbox("enable backtrack", &legit_backtrack);
		ImGui::Checkbox("enable triggerbot", &legit_triggerbot);
		ImGui::SliderInt("triggerbot hitchance", &triggerbot_hitchance, 0, 100);
		ImGui::SliderInt("triggerbot delay", &triggerbot_delay, 0, 500, "%.0f ms");
		ImGui::Checkbox("silent aim", &legit_silent_aim);
		ImGui::Checkbox("visible only", &legit_visible_check);
	}
	ImGui::EndChild(true);

	// 2nd column
	ImGui::NextColumn();
	ImGui::BeginChild(tab_name.c_str(), ImVec2(229, 442), true);
	{
		ImGui::SliderFloat("fov", &legit_fov[legit_weapon_setting], 0, 10, "%.2f \xC2\xB0");

		ImGui::SliderInt("smooth", &legit_smooth[legit_weapon_setting], 1, 100);

		ImGui::SliderInt("rcs amount", &legit_rcs[legit_weapon_setting], 1, 100);

		if (legit_backtrack)
		{
			ImGui::Checkbox("aim at backtrack", &legit_aim_at_backtrack[legit_weapon_setting]);
			ImGui::SliderInt("backtrack ticks", &legit_backtrack_ticks[legit_weapon_setting], 0, 10);
		}

		ImGui::Text("aimbot hitboxes");
		ImGui::ListBoxHeader(("##aimbot hitboxes"), ImVec2(-1, 108));
		ImGui::Selectable("head", &hitboxes[0][legit_weapon_setting]);
		ImGui::Selectable("neck", &hitboxes[1][legit_weapon_setting]);
		ImGui::Selectable("chest", &hitboxes[2][legit_weapon_setting]);
		ImGui::Selectable("stomach", &hitboxes[3][legit_weapon_setting]);
		ImGui::Selectable("pelvis", &hitboxes[4][legit_weapon_setting]);
		ImGui::ListBoxFooter();

		ImGui::Text("triggerbot hitboxes");
		ImGui::ListBoxHeader(("##triggerbot hitboxes"), ImVec2(-1, 65));
		ImGui::Selectable("head", &trigger_hitboxes[0][legit_weapon_setting]);
		ImGui::Selectable("chest", &trigger_hitboxes[1][legit_weapon_setting]);
		ImGui::Selectable("stomach", &trigger_hitboxes[2][legit_weapon_setting]);
		ImGui::ListBoxFooter();
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();
	ImGui::Columns(1);
}

void misc_tab()
{
	// 1st column
	ImGui::Columns(2, nullptr, false);

	fix_child();
	ImGui::BeginChild("visuals", ImVec2(229, 442), true);
	{
		// visuals
		ImGui::Checkbox("enable chams", &chams_enable);
		ImGui::ColorEdit4("chams vis color", chams_vis_color, ImGuiColorEditFlags_NoInputs);

		ImGui::Checkbox("chams nonvisible", &chams_enable_behind);
		ImGui::ColorEdit4("chams invis color", chams_invis_color, ImGuiColorEditFlags_NoInputs);
	}
	ImGui::EndChild(true);

	// 2nd column
	ImGui::NextColumn();
	ImGui::BeginChild("misc", ImVec2(229, 442), true);
	{
		ImGui::Checkbox("enable bhop", &misc_bhop);

		ImGui::SliderInt("bhop hitchance", &misc_bhop_hitchance, 0, 100);

		ImGui::SliderInt("bhop max hits", &misc_bhop_max_hits, 0, 10);

		ImGui::SliderInt("bhop allowed misses", &misc_bhop_allowed_misses, 0, 10);
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();
	ImGui::Columns(1);
}

void skin_tab()
{
	static int cur_weapon;
	std::string weapon_str = get_name(cur_weapon);
	std::vector<weapon_name> weapons, skins;

	// copy vectors
	{
		for (int i = 0; i < game_data::weap_names.size(); i++)
			weapons.push_back(game_data::weap_names[i]);

		for (auto skin : game_data::weapon_skins[weapon_str])
			skins.push_back({ game_data::skin_map[skin].paintkit, game_data::skin_names[game_data::skin_map[skin].name] });
	}

	// alphabetize items
	{
		std::sort(weapons.begin(), weapons.end(), compare_alphabet);
		std::sort(skins.begin(), skins.end(), compare_alphabet);
	}

	// 2 columns
	ImGui::Columns(2, nullptr, false);

	fix_child();
	ImGui::BeginChild("weapons", ImVec2(229, 340), true);
	{
		for (auto it : weapons)
		{
			if (it.definition_index >= 500)
				continue;

			std::string name = it.name;
			bool item_selected = it.definition_index == cur_weapon;
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (ImGui::Selectable(name.c_str(), item_selected))
				cur_weapon = it.definition_index;
		}
	}
	ImGui::EndChild(true);

	// select skins
	ImGui::NextColumn();
	ImGui::BeginChild("skins", ImVec2(229, 340), true);
	{
		for (auto it : skins)
		{
			auto skin_str = it.name;
			auto skin_id = it.definition_index;
			auto selected = (game_data::w_settings[cur_weapon].skin == skin_id);
			std::transform(skin_str.begin(), skin_str.end(), skin_str.begin(), ::tolower);

			if (ImGui::Selectable(skin_str.c_str(), selected))
			{
				game_data::w_settings[cur_weapon].skin = skin_id;
				(*g_client_state)->ForceFullUpdate();
				break;
			}
		}
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();
	ImGui::Columns(1, nullptr, false);

	fix_child();
	ImGui::BeginChild("settings", ImVec2(468, 85), true);
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::InputInt("seed", &w_settings[cur_weapon].seed);
		ImGui::InputInt("stattrack", &w_settings[cur_weapon].stat_track);

		ImGui::NextColumn();
		ImGui::SliderFloat("wear", &w_settings[cur_weapon].wear, FLT_MIN, 1.f, "%.5f", 5);

		if (ImGui::Button("force update", ImVec2(225, 25)))
			(*g_client_state)->ForceFullUpdate();
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();
}

void model_tab()
{
	int knife = knife_model, glove = glove_model;
	std::vector<weapon_name> knives, gloves, knife_skins, glove_skins;
	std::string knife_str = get_name(knife_names[knife].definition_index), glove_str = get_name(glove_names[glove].definition_index);

	// copy knife names
	for (auto i : knife_names)
		knives.push_back(i);

	// copy glove names
	for (auto i : glove_names)
		gloves.push_back(i);

	for (auto skin : weapon_skins[knife_str])
		knife_skins.push_back({ skin_map[skin].paintkit, skin_names[skin_map[skin].name] });

	for (auto skin : weapon_skins[glove_str])
		glove_skins.push_back({ skin_map[skin].paintkit, skin_names[skin_map[skin].name] });

	// alphabetize items
	std::sort(knives.begin() + 1, knives.end(), compare_alphabet);
	std::sort(gloves.begin() + 2, gloves.end(), compare_alphabet);
	std::sort(knife_skins.begin(), knife_skins.end(), compare_alphabet);
	std::sort(glove_skins.begin(), glove_skins.end(), compare_alphabet);

	// setup our columns.
	ImGui::Columns(2, nullptr, false);

	// knives child window.
	fix_child();
	ImGui::BeginChild("knives", ImVec2(229, 170), true);
	{
		for (auto it : knives)
		{
			std::string name = it.name;
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (ImGui::Selectable(name.c_str(), it.menu_value == knife_model))
				knife_model = it.menu_value;
		}
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();

	// gloves child window.
	fix_child();
	ImGui::BeginChild("gloves", ImVec2(229, 162), true);
	{
		for (auto it : gloves)
		{
			std::string name = it.name;
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (ImGui::Selectable(name.c_str(), it.menu_value == glove_model))
				glove_model = it.menu_value;
		}
	}
	ImGui::EndChild(true);

	// RIGHT SIDE OF MENU!
	ImGui::NextColumn();

	// knife skin child window.
	ImGui::BeginChild("knife skins", ImVec2(229, 170), true);
	{
		for (auto skin : knife_skins)
		{
			auto skin_str = skin.name;
			auto skin_id = skin.definition_index;

			if (skin_id >= 415 && skin_id <= 421 || skin_id >= 568 && skin_id <= 572 || skin_id >= 617 && skin_id <= 619)
				skin_str += " | " + std::to_string(skin_id);

			std::transform(skin_str.begin(), skin_str.end(), skin_str.begin(), ::tolower);

			if (ImGui::Selectable(skin_str.c_str(), w_settings[knife_names[knife_model].definition_index].skin == skin_id))
			{
				w_settings[knife_names[knife_model].definition_index].skin = skin_id;
				(*g_client_state)->ForceFullUpdate();
				break;
			}
		}
	}
	ImGui::EndChild(true);

	// glove skin child window.
	ImGui::BeginChild("glove skins", ImVec2(229, 162), true);
	{
		for (auto skin : glove_skins)
		{
			auto skin_str = skin.name;
			auto skin_id = skin.definition_index;
			std::transform(skin_str.begin(), skin_str.end(), skin_str.begin(), ::tolower);

			if (ImGui::Selectable(skin_str.c_str(), w_settings[glove_names[glove_model].definition_index].skin == skin_id))
			{
				w_settings[glove_names[glove_model].definition_index].skin = skin_id;
				(*g_client_state)->ForceFullUpdate();
				break;
			}
		}
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();
	ImGui::Columns(1, nullptr, false);

	// settings child window.
	fix_child();
	ImGui::BeginChild("knife customizations", ImVec2(468, 85), true);
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::InputInt("seed", &w_settings[knife_names[knife_model].definition_index].seed);
		ImGui::InputInt("stattrack", &w_settings[knife_names[knife_model].definition_index].stat_track);

		ImGui::NextColumn();
		ImGui::SliderFloat("wear", &w_settings[knife_names[knife_model].definition_index].wear, FLT_MIN, 1.f, "%.5f", 5);

		if (ImGui::Button("force update", ImVec2(225, 25)))
			(*g_client_state)->ForceFullUpdate();
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();
}

void config_tab()
{
	static int cur_config = -1;
	static char config_name[128] = "default";
	static std::vector<std::string> config_items = config::get_configs();

	std::string f_path;
	if (config_items.size() > 0 && cur_config >= 0)
		f_path = std::string(game_data::config_path) + config_items[cur_config] + ".brood";

	// setup our columns.
	ImGui::Columns(2, nullptr, false);

	// config list child window.
	fix_child();
	ImGui::BeginChild("info", ImVec2(229, 442), true);
	{
		ImGui::InputText("", config_name, 206);

		// render all of the configs in folder.
		for (int i = 0; i < config_items.size(); i++)
		{
			std::string path = std::string(game_data::config_folder) + "\\skins\\" + config_items[i] + ".brood";
			if (ImGui::Selectable(config_items[i].c_str(), cur_config == i))
			{
				cur_config = i;
				config::load_config(path);
				break;
			}
		}
	}
	ImGui::EndChild(true);

	// RIGHT SIDE
	ImGui::NextColumn();

	// config settings child window.
	ImGui::BeginChild("config", ImVec2(229, 217), true);
	{
		// refresh configs
		if (ImGui::Button("refresh", ImVec2(97, 25)))
			config_items = config::get_configs();

		ImGui::SameLine();

		// save selected config.
		if (ImGui::Button("save", ImVec2(97, 25)) && config_items.size() > 0 && (cur_config >= 0 && cur_config < (int)config_items.size()))
			config::save_config(f_path);

		// remove selected config
		if (ImGui::Button("remove", ImVec2(97, 25)) && config_items.size() > 0 && (cur_config >= 0 && cur_config < (int)config_items.size()))
		{
			cur_config = -1;
			std::remove(f_path.c_str());
			config_items = config::get_configs();
		}

		ImGui::SameLine();

		// create new config
		if (ImGui::Button("add", ImVec2(97, 25)))
		{
			cur_config = -1;
			config_items = config::get_configs();
			config::save_config(std::string(game_data::config_path) + config_name + ".brood");
		}
	}
	ImGui::EndChild(true);

	// settings child window.
	ImGui::BeginChild("settings", ImVec2(229, 217), true);
	{
		ImGui::Text("menu color");
		ImGui::ColorEdit4("chams invis color", menu_color, ImGuiColorEditFlags_NoInputs);
	}
	ImGui::EndChild(true); ImGui::PopStyleVar();

	ImGui::Columns();
}

void draw_gui()
{
	// load menu color
	auto& style = ImGui::GetStyle();
	auto color = ImVec4(menu_color[0], menu_color[1], menu_color[2], menu_color[3]);
	style.Colors[ImGuiCol_CheckMark] = color;
	style.Colors[ImGuiCol_SliderGrab] = color;
	style.Colors[ImGuiCol_SliderGrabActive] = color;

	// setup font
	ImGui::PushFont(render::font_menu);

	// begin menu
	ImGui::SetNextWindowSize(ImVec2(500, 500));
	ImGui::Begin("fedorahook", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
	{
		// tab bar
		auto cur_pos = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cur_pos.x, cur_pos.y + 30), ImVec2(cur_pos.x + 620, cur_pos.y - 3), ImColor(30, 30, 39));

		//  this is really ghetto and i should implement proper gradient bar but it works so fuck you :)
		auto width_div_three = ImGui::GetWindowWidth() / 3;
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(cur_pos.x, cur_pos.y + 32), ImVec2(cur_pos.x + width_div_three, cur_pos.y + 30),
			ImColor(252, 3, 3, 255), ImColor(252, 248, 3, 255), ImColor(252, 248, 3, 255), ImColor(252, 3, 3, 255));

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(cur_pos.x + width_div_three, cur_pos.y + 32), ImVec2(cur_pos.x + width_div_three * 2, cur_pos.y + 30),
			ImColor(252, 248, 3, 255), ImColor(0, 255, 17, 255), ImColor(0, 255, 17, 255), ImColor(252, 248, 3, 255));

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(cur_pos.x + width_div_three * 2, cur_pos.y + 32), ImVec2(cur_pos.x + width_div_three * 3, cur_pos.y + 30),
			ImColor(0, 255, 17, 255), ImColor(0, 68, 255, 255), ImColor(0, 68, 255, 255), ImColor(0, 255, 17, 255));

		// render title
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
		ImGui::Text("fedorahook");
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);

		// handle tabs
		static int page = PAGE_LEGITBOT;
		if (tab("legit", ImVec2(40, 30), &page, PAGE_LEGITBOT, color, false));
		if (tab("misc", ImVec2(40, 30), &page, PAGE_MISC, color, false));
		if (tab("skins", ImVec2(40, 30), &page, PAGE_SKINS, color, false));
		if (tab("models", ImVec2(40, 30), &page, PAGE_MODELS, color, false));
		if (tab("config", ImVec2(40, 30), &page, PAGE_CONFIG, color, false));

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 45);

		// setup menu colors
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));

		// render tab.
		switch (page) {
		case PAGE_LEGITBOT: settings_tab(); break;
		case PAGE_MISC: misc_tab(); break;
		case PAGE_SKINS: skin_tab(); break;
		case PAGE_MODELS: model_tab(); break;
		case PAGE_CONFIG: config_tab(); break;
		}
		ImGui::PopStyleColor(2);
	}

	ImGui::End();
	ImGui::PopFont();
}
