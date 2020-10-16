#include "../Hooks/hooks.hpp"
#include "../main.hpp"
#include "../Utilities/config.hpp"
#include "../SDK/IMaterials.hpp"
#include <Windows.h>

using namespace game_data;
hooks::DrawModelExecute::Fn* hooks::DrawModelExecute::m_original;
void __fastcall hooks::DrawModelExecute::hooked(void* thisptr, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* custom_bone)
{
	if (g_model_render->IsForcedMaterialOverride() && !strstr(info.pModel->name, "arms") &&!strstr(info.pModel->name, "weapons/v_")) 
		return m_original(thisptr, edx, ctx, state, info, custom_bone);

	sdk::IMaterial* material_vis = g_material->find_material("debug/debugambientcube", nullptr);
	sdk::IMaterial* material_invis = g_material->find_material("debug/debugambientcube", nullptr);

	if (!material_vis || !material_invis || info.entity_index > 64)
		return m_original(thisptr, edx, ctx, state, info, custom_bone);

	auto model_name = g_model_info->GetModelName((sdk::model_t*)info.pModel);
	auto cur_ent = static_cast<sdk::C_BasePlayer*>(g_entity_list->GetClientEntity(info.entity_index));
	
	if (!cur_ent || !g_local)
		return m_original(thisptr, edx, ctx, state, info, custom_bone);

	if (game_data::chams_enable && cur_ent->GetTeamNumber() != g_local->GetTeamNumber() && strstr(model_name, "models/player"))
	{
		if (game_data::chams_enable_behind)
		{
			material_invis->set_color(chams_invis_color[0], chams_invis_color[1], chams_invis_color[2]);
			material_invis->SetMaterialVarFlag(sdk::MATERIAL_VAR_IGNOREZ, true);
			g_model_render->ForcedMaterialOverride(material_invis);
			m_original(thisptr, edx, ctx, state, info, custom_bone);
		}

		material_vis->set_color(chams_vis_color[0], chams_vis_color[1], chams_vis_color[2]);
		material_invis->SetMaterialVarFlag(sdk::MATERIAL_VAR_IGNOREZ, false);
		g_model_render->ForcedMaterialOverride(material_vis);
	}

	m_original(thisptr, edx, ctx, state, info, custom_bone);
	g_model_render->ForcedMaterialOverride(nullptr);
}