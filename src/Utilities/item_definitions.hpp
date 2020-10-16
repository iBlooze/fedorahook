#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
enum ItemDefinitionIndex : int
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5_SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

static inline auto is_knife(const int i) -> bool
{
	return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
}

//extern const std::map<size_t, weapon_info> k_weapon_info;
namespace game_data
{
	// Stupid MSVC requires separate constexpr constructors for any initialization
	struct weapon_info
	{
		constexpr weapon_info(const char* model, const char* icon = nullptr) :
			model(model),
			icon(icon)
		{}

		const char* model;
		const char* icon;
	};

	struct weapon_name
	{
		int definition_index = 0;
		std::string name;
		int menu_value = 0;
	};

	struct quality_name
	{
		constexpr quality_name(const int index, const char* name) :
			index(index),
			name(name)
		{}

		int index = 0;
		const char* name = nullptr;
	};

	struct weapon_settings
	{
		int skin = 0;		
		int stat_track = 0;
		int seed = 0;
		int model = 0;
		float wear = 0;
		char* name = "";
	}; 

	struct paint_kit
	{
		int seed = -1;
		int paintkit;
		std::string name;
	};

	struct Item_t
	{
		constexpr Item_t(const char* model, const char* icon = nullptr) : model(model), icon(icon)
		{}

		const char* model;
		const char* icon;
	};

	extern bool chams_enable, chams_enable_behind,
				legit_enable, legit_rcs_enable, legit_backtrack, legit_visible_check, legit_silent_aim, legit_triggerbot, hitboxes[5][3], trigger_hitboxes[5][3], legit_aim_at_backtrack[5],
				misc_bhop, misc_auto_accept, misc_edge_jump;

	extern int  knife_model, glove_model, 
				legit_weapon_setting, triggerbot_hitchance, triggerbot_delay, legit_smooth[5], legit_rcs[5], legit_backtrack_ticks[5],
				misc_bhop_hitchance, misc_bhop_allowed_misses, misc_bhop_max_hits;

	extern float legit_fov[5];
	extern float menu_color[5], chams_vis_color[5], chams_invis_color[5];

	extern std::string config_path;
	extern char config_folder[260];
	extern weapon_settings w_settings[5040];

	using namespace std;
	const weapon_info* get_weapon_info(int defindex);
	extern const char* get_name(int id);
	extern std::vector<weapon_name> knife_names;
	extern std::vector<weapon_name> glove_names;
	extern std::vector<weapon_name> model_names;
	extern std::vector<weapon_name> weap_names;
	extern std::vector<quality_name> quality_names;

	extern unordered_map<string, set<string>> weapon_skins;
	extern unordered_map<string, game_data::paint_kit> skin_map;
	extern unordered_map<string, string> skin_names;
	extern vector<string> weapon_data;

}