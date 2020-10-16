#include "legitbot.hpp"

// move this you lazy nigger
#define TIME_TO_TICKS( dt )	( (int)( 0.5f + (float)(dt) / g_global_vars->interval_per_tick ) )

// runs aimbot.
using namespace game_data;
auto legitbot::run_aimbot(sdk::CUserCmd* pCmd) -> void
{
	// not holding weapon
	if (cur_weapon_index < 0)
		return;

	// loop through players
	new_time = 0.f;
	aim_angle = QAngle(0, 0, 0);
	for (int it = 1; it <= 64; it++)
	{
		auto cur_record = player_data{ 0, Vector(0, 0, 0) };
		auto cur_ent = static_cast<sdk::C_BasePlayer*>(g_entity_list->GetClientEntity(it));
		if (!cur_ent)
			continue;

		if (!hooks::valid_entity(cur_ent) || (legit_visible_check && !visible_check(cur_ent)))
		{
			backtrack_data[it].clear();
			continue;
		}

		// save backtrack info
		cur_record.save_record(cur_ent);
		backtrack_data[it].push_back(cur_record);

		if (backtrack_data[it].size() > legit_backtrack_ticks[cur_weapon_index])
			backtrack_data[it].erase(backtrack_data[it].begin());

		// find best aimpoint
		process_player(cur_ent);
	}

	// do backtrack on our player.
	process_player(nullptr);

	// if no player / records within 10fov then return.
	if (!backtrack_player)
		return;

	// aim and restore time if shooting.
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		// do the aims
		if (legit_enable && aim_angle != QAngle(0, 0, 0) && aimbot_player)
			aim_at_target(pCmd, aim_angle);

		// do the timessss
		if (legit_backtrack && new_time)
			pCmd->tick_count = TIME_TO_TICKS(new_time);
	}
}

// run bt/aimbot on a player
auto legitbot::process_player(sdk::C_BasePlayer* cur_ent) -> void
{
	auto fov = 360.f;
	auto view_angles = QAngle(0, 0, 0);
	g_engine->GetViewAngles(view_angles);

	// loop all hitboxes
	for (auto i : aimbot_hitboxes)
	{
		if (hitboxes[i.second][cur_weapon_index] == false || !cur_ent)
			continue;

		auto ang = math::calc_angle(g_local->GetEyePosition(), hitbox_pos(cur_ent, i.first));
		if (legit_rcs_enable)
			ang -= (g_local->GetAimPunchAngle() * ((float)legit_rcs[cur_weapon_index] / 50));

		math::clamp_angles(ang);
		auto fov_to_ang = math::get_fov(view_angles, ang);
		if (fov_to_ang < fov)
		{
			fov = fov_to_ang;
			if (fov_to_ang <= legit_fov[cur_weapon_index])
			{
				aim_angle = ang;
				aimbot_player = cur_ent->EntIndex();
			}

			if (fov_to_ang <= 10)
			{
				new_time = cur_ent->GetSimTime();
				backtrack_player = cur_ent->EntIndex();
			}
		}
	}

	// when doing backtrack shit we pass nullptr here.
	fov = 360.f;
	if (cur_ent)
		return;

	// loop all backtrack records
	for (auto& data : backtrack_data[backtrack_player])
	{
		auto ang = math::calc_angle(g_local->GetEyePosition(), data.head_pos) - (g_local->GetAimPunchAngle() * ((float)legit_rcs[cur_weapon_index] / 50));

		math::clamp_angles(ang);
		auto fov_to_ang = math::get_fov(view_angles, ang);
		if (fov_to_ang < fov && fov_to_ang <= 10 && legit_backtrack)
		{
			fov = fov_to_ang;
			new_time = data.sim_time;

			if (legit_aim_at_backtrack[cur_weapon_index] && fov_to_ang <= legit_fov[cur_weapon_index])
				aim_angle = ang;
		}
	}
}

// setup weapon configs
auto legitbot::sync_settings(sdk::CUserCmd* pCmd) -> void
{
	auto weapon = (sdk::C_BaseCombatWeapon*)g_entity_list->GetClientEntityFromHandle(g_local->GetWeaponActive());
	if (!weapon)
		return;

	if (weapon->GetClip1() < 1)
		cur_weapon_index = -1;

	// make this not ghetto lmfao....
	short id = weapon->Index();
	static const std::vector<int> snipers = { WEAPON_AWP, WEAPON_SSG08, WEAPON_G3SG1, WEAPON_SCAR20, WEAPON_NOVA, WEAPON_XM1014, WEAPON_SAWEDOFF, WEAPON_MAG7 };
	static const std::vector<int> pistols = { WEAPON_GLOCK, WEAPON_ELITE, WEAPON_P250, WEAPON_TEC9, WEAPON_CZ75A, WEAPON_DEAGLE, WEAPON_REVOLVER, WEAPON_USP_SILENCER, WEAPON_HKP2000, WEAPON_FIVESEVEN };
	static const std::vector<int> automatic = { WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,WEAPON_M249,WEAPON_M4A1,WEAPON_M4A1_SILENCER,WEAPON_NEGEV,WEAPON_SG556,WEAPON_M249, WEAPON_NEGEV,  WEAPON_MAC10, WEAPON_MP7, WEAPON_UMP45, WEAPON_P90, WEAPON_BIZON, WEAPON_MP9, WEAPON_MP5_SD };
	if (std::find(pistols.begin(), pistols.end(), id) != pistols.end())
		cur_weapon_index = 0;

	if (std::find(snipers.begin(), snipers.end(), id) != snipers.end())
		cur_weapon_index = 2;

	if (std::find(automatic.begin(), automatic.end(), id) != automatic.end())
		cur_weapon_index = 1;

	// we will do it here 4 now.
	if (legit_triggerbot)
		triggerbot(pCmd, weapon);
}

using namespace sdk;
auto legitbot::triggerbot(CUserCmd* pCmd, C_BaseCombatWeapon* pWeapon) -> void
{
	Ray_t ray;
	trace_t tr;
	QAngle view_angles;
	Vector end, angles;
	CTraceFilter filter;
	static size_t delay = 0;
	auto time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

	// our viewangles
	g_engine->GetViewAngles(view_angles);
	view_angles += g_local->GetAimPunchAngle() * 2.f;
	math::angle_vectors(view_angles, angles);

	// setup ray info
	angles *= 8192.f;
	end = g_local->GetEyePosition() + angles;

	// run traceray
	filter.pSkip = g_local;
	ray.Init(g_local->GetEyePosition(), end);
	g_engine_trace->TraceRay(ray, 0x4600400B, &filter, &tr);

	auto hit_ent = static_cast<sdk::C_BasePlayer*>(tr.hit_entity);
	auto hitchanced = hit_chance(tr.hitbox, hit_ent, triggerbot_hitchance);

	// see if entity is valid
	if (!hit_ent ||
		hit_ent->GetTeamNumber() == g_local->GetTeamNumber() ||
		hit_ent->GetLifeState() != LifeState::ALIVE)
		return;

	// check hitchance & hitgroups
	if (hitchanced && (trigger_hitboxes[0][cur_weapon_index] && tr.hitgroup == HITGROUP_HEAD || trigger_hitboxes[1][cur_weapon_index] && tr.hitgroup == HITGROUP_CHEST || trigger_hitboxes[2][cur_weapon_index] && tr.hitgroup == HITGROUP_STOMACH) && GetAsyncKeyState(VK_LMENU))
	{
		// see if we shoot / set the delay
		if (time >= delay && delay != 0)
		{
			delay = time + delay;
			pCmd->buttons |= IN_ATTACK;
		}
		else if (!delay)
			delay = time + delay;
	}
	else
		delay = 0;
}

// ghetto as fuck just doing it for now... couldn't figure out why proper way wasnt working credits to UC for this.
auto legitbot::hit_chance(int hitbox, sdk::C_BasePlayer* ent, float chance) -> bool
{
	auto multiplier = 1.f, low = 0.0000001f, inaccuracy = 0.f;
	auto distance = ent->GetOrigin().DistTo(g_local->GetOrigin());
	auto weapon = (sdk::C_BaseCombatWeapon*)g_entity_list->GetClientEntityFromHandle(g_local->GetWeaponActive());
	if (!weapon)
		return false;

	// setup size multiplier
	switch (hitbox)
	{
	case HITBOX_CHEST:
	case HITBOX_UPPER_CHEST:
	case HITBOX_PELVIS:
	case HITBOX_STOMACH:
		multiplier = 2;
		break;
	case HITBOX_RIGHT_THIGH:
	case HITBOX_LEFT_THIGH:
		multiplier = 1.5f;
		break;
	}

	weapon->UpdateAccuracyPenalty();
	inaccuracy = weapon->GetInaccuracy();

	// we dont want this 0, clamping max 100.f cuz idk the true max.
	std::clamp(inaccuracy, 0.0000001f, 100.f);

	// yeah... ghetto.....
	return (5.1432 / (sqrt(tan(inaccuracy * 3.932f) * distance))) * 100.f * multiplier >= chance;
}

// also credits to UC for this
auto legitbot::run_bhop(sdk::CUserCmd* cmd) -> void
{
	static int hops_restricted = 0, hops_hit = 0;
	if (!misc_bhop || !(cmd->buttons & IN_JUMP) || (g_local->GetMoveType() & sdk::MOVETYPE_LADDER))
		return;

	if (!(g_local->GetFlags() & sdk::FL_ONGROUND))
	{
		cmd->buttons &= ~IN_JUMP;
		hops_restricted = 0;
	}
	else if ((rand() % 100 > misc_bhop_hitchance && hops_restricted < misc_bhop_allowed_misses)
		|| (misc_bhop_max_hits > 0 && hops_hit > misc_bhop_max_hits))
	{
		cmd->buttons &= ~IN_JUMP;
		hops_restricted++;
		hops_hit = 0;
	}
	else
		hops_hit++;
}

// saves BT data, will add more to this
void legitbot::player_data::save_record(sdk::C_BasePlayer* ent)
{
	sim_time = ent->GetSimTime();
	head_pos = ent->BonePosition(8);
}

// check if we see dem
auto legitbot::visible_check(sdk::C_BasePlayer* player) -> bool
{
	// trace data
	sdk::Ray_t ray;
	sdk::CGameTrace tr;
	sdk::CTraceFilter filter;
	filter.pSkip = g_local;

	// other defs
	matrix3x4_t matrix[MAXSTUDIOBONES];
	auto studio_model = g_model_info->GetStudiomodel(player->GetModel());

	// setup bones
	if (!player->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f) || !studio_model)
		return false;

	// loop all hitboxes
	for (int i = 0; i < ARRAYSIZE(scan_hitboxes); i++)
	{
		auto hitbox = studio_model->GetHitboxSet(player->HitboxSet())->GetHitbox(scan_hitboxes[i]);
		if (!hitbox)
			continue;

		auto min = Vector{}, max = Vector{};
		math::vector_transform(hitbox->mins, matrix[hitbox->bone], min);
		math::vector_transform(hitbox->maxs, matrix[hitbox->bone], max);

		// run trace
		ray.Init(g_local->GetEyePosition(), (min + max) * 0.5);
		g_engine_trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		// if we hit a player & they are visible
		if (tr.hit_entity == player || tr.fraction > 0.97f)
			return true;
	}
	return false;
}

// set de view angles
auto legitbot::aim_at_target(sdk::CUserCmd* pCmd, QAngle aim_angle) -> void
{
	QAngle viewangles;
	g_engine->GetViewAngles(viewangles);
	static auto smooth_angle = [](QAngle src, QAngle& dst, float factor) -> void
	{
		QAngle delta = dst - src;
		math::clamp_angles(delta);
		dst = src + delta / factor;
	};

	auto aim_smooth = math::clamp(legit_smooth[cur_weapon_index], 1, 100);
	smooth_angle(viewangles, aim_angle, aim_smooth);

	// clamp to prevent weird shit.
	math::clamp_angles(aim_angle);
	pCmd->viewangles = aim_angle;

	if (!legit_silent_aim)
		g_engine->SetViewAngles(pCmd->viewangles);
}

// for someone reason 
auto legitbot::hitbox_pos(sdk::C_BasePlayer* player, int hitbox) -> Vector
{
	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!player->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
		return Vector(0, 0, 0);

	auto studio_model = g_model_info->GetStudiomodel(player->GetModel());
	if (!studio_model)
		return Vector(0, 0, 0);

	auto hitbox_set = studio_model->GetHitboxSet(player->HitboxSet())->GetHitbox(hitbox);
	if (!hitbox_set)
		return Vector(0, 0, 0);

	auto min = Vector{},
		max = Vector{};

	math::vector_transform(hitbox_set->mins, matrix[hitbox_set->bone], min);
	math::vector_transform(hitbox_set->maxs, matrix[hitbox_set->bone], max);

	return (min + max) * 0.5;

}