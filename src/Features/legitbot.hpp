#include "../Utilities/math.hpp"
#include "../Utilities/config.hpp"
#include "../main.hpp"
#include "../Hooks/hooks.hpp"

namespace legitbot
{
	struct player_data
	{
		Vector head_pos;
		float sim_time = 0.f;

		player_data(float sim_time, Vector head_pos)
		{
			sim_time = sim_time;
			head_pos = head_pos;
		}

		void save_record(sdk::C_BasePlayer* ent);
	};

	static std::vector<player_data> backtrack_data[64];

	static int scan_hitboxes[] = {
		HITBOX_HEAD,
		HITBOX_LEFT_FOREARM,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_LEFT_FOOT,
		HITBOX_RIGHT_FOOT,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF,
		HITBOX_STOMACH,
		HITBOX_CHEST
	};

	const static std::map<int, int> aimbot_hitboxes =
	{
		{HITBOX_HEAD, 0},
		{HITBOX_NECK, 1},
		{HITBOX_CHEST, 2},
		{HITBOX_STOMACH, 3},
		{HITBOX_PELVIS, 4}
	};

	auto hitbox_pos(sdk::C_BasePlayer* player, int hitbox)->Vector;
	auto sync_settings(sdk::CUserCmd* pCmd) -> void;
	auto run_aimbot(sdk::CUserCmd* pCmd) -> void;
	auto process_player(sdk::C_BasePlayer* cur_ent) -> void;
	auto triggerbot(sdk::CUserCmd* pCmd, sdk::C_BaseCombatWeapon* pWeapon) -> void;
	auto hit_chance(int hitgroup, sdk::C_BasePlayer* ent, float chance) -> bool;
	auto run_bhop(sdk::CUserCmd* cmd) -> void;
	auto visible_check(sdk::C_BasePlayer* player) -> bool;
	auto aim_at_target(sdk::CUserCmd* pCmd, QAngle aim_angle) -> void;

	static QAngle aim_angle;
	static float new_time = 0.f;
	static int cur_weapon_index, aimbot_player, backtrack_player;

}