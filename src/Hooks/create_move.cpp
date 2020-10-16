#include "create_move.hpp"
#include "../Features/legitbot.hpp"

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / g_global_vars->interval_per_tick ) )

hooks::CreateMove::Fn* hooks::CreateMove::m_original;

bool __fastcall hooks::CreateMove::hooked(sdk::IClientMode* thisptr, void* edx, float sample_frametime, sdk::CUserCmd* cmd)
{
	if (!cmd || !cmd->command_number || !g_local || g_local->GetLifeState() != sdk::LifeState::ALIVE)
		return m_original;

	m_original(thisptr, edx, sample_frametime, cmd);
	if (cmd->command_number == 0)
		return false;

	if ((cmd->buttons & IN_SCORE) != 0)
		g_client->DispatchUserMessage(50, 0, 0, nullptr);

	// sync shit
	legitbot::sync_settings(cmd);

	// do legitbot
	legitbot::run_aimbot(cmd);

	// do bhop
	legitbot::run_bhop(cmd);

	// no untrusted ban here.
	math::clamp_angles(cmd->viewangles);
	return false;
}
