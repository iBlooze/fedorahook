#include "item_definitions.hpp"

// integer
int game_data::knife_model = 1;
int game_data::glove_model = 1;

int game_data::legit_rcs[5];
int game_data::legit_smooth[5];
int game_data::legit_backtrack_ticks[5];
int game_data::legit_weapon_setting;
int game_data::triggerbot_hitchance;
int game_data::triggerbot_delay;

int game_data::misc_bhop_hitchance;
int game_data::misc_bhop_allowed_misses;
int game_data::misc_bhop_max_hits;

// bool
bool game_data::legit_enable;
bool game_data::legit_rcs_enable;
bool game_data::legit_backtrack;
bool game_data::legit_visible_check;
bool game_data::legit_silent_aim;
bool game_data::legit_triggerbot;

bool game_data::hitboxes[5][3];
bool game_data::trigger_hitboxes[5][3];
bool game_data::legit_aim_at_backtrack[5];

bool game_data::misc_bhop;
bool game_data::misc_auto_accept;
bool game_data::misc_edge_jump;

bool game_data::chams_enable;
bool game_data::chams_enable_behind;

// float
float game_data::legit_fov[5];

float game_data::menu_color[5] = {1.f, 0.f, 0.f, 1.f};
float game_data::chams_vis_color[5];
float game_data::chams_invis_color[5];

// other
char game_data::config_folder[260];
std::string game_data::config_path;

// game_data defines
using namespace std;
game_data::weapon_settings game_data::w_settings[5040];
unordered_map<string, set<string>> game_data::weapon_skins;
unordered_map<string, game_data::paint_kit> game_data::skin_map;
unordered_map<string, string> game_data::skin_names;
vector<string> game_data::weapon_data;

// We need these for overriding viewmodels and icons
const game_data::weapon_info* game_data::get_weapon_info(int defindex)
{
	const static map<int, weapon_info> info =
	{
		{WEAPON_KNIFE,{"models/weapons/v_knife_default_ct.mdl", "knife_default_ct"}},
		{WEAPON_KNIFE_BAYONET, {"models/weapons/v_knife_bayonet.mdl", "bayonet"}},
		{WEAPON_KNIFE_CSS, {"models/weapons/v_knife_css.mdl", "knife_css"}},
		{WEAPON_KNIFE_FLIP, {"models/weapons/v_knife_flip.mdl", "knife_flip"}},
		{WEAPON_KNIFE_GUT, {"models/weapons/v_knife_gut.mdl", "knife_gut"}},
		{WEAPON_KNIFE_KARAMBIT, {"models/weapons/v_knife_karam.mdl", "knife_karambit"}},
		{WEAPON_KNIFE_M9_BAYONET, {"models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet"}},
		{WEAPON_KNIFE_TACTICAL, {"models/weapons/v_knife_tactical.mdl", "knife_tactical"}},
		{WEAPON_KNIFE_FALCHION, {"models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion"}},
		{WEAPON_KNIFE_SURVIVAL_BOWIE, {"models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie"}},
		{WEAPON_KNIFE_BUTTERFLY, {"models/weapons/v_knife_butterfly.mdl", "knife_butterfly"}},
		{WEAPON_KNIFE_PUSH, {"models/weapons/v_knife_push.mdl", "knife_push"}},
		{WEAPON_KNIFE_CORD, {"models/weapons/v_knife_cord.mdl", "knife_cord"}},
		{WEAPON_KNIFE_CANIS, {"models/weapons/v_knife_canis.mdl", "knife_canis"}},
		{WEAPON_KNIFE_URSUS,{"models/weapons/v_knife_ursus.mdl", "knife_ursus"}},
		{WEAPON_KNIFE_GYPSY_JACKKNIFE,{"models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife"}},
		{WEAPON_KNIFE_OUTDOOR,{"models/weapons/v_knife_outdoor.mdl", "knife_outdoor"}},
		{WEAPON_KNIFE_STILETTO,{"models/weapons/v_knife_stiletto.mdl", "knife_stiletto"}},
		{WEAPON_KNIFE_WIDOWMAKER,{"models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker"}},
		{WEAPON_KNIFE_SKELETON,{"models/weapons/v_knife_skeleton.mdl", "knife_skeleton"}},
		{GLOVE_STUDDED_BLOODHOUND,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"}},
		{GLOVE_T_SIDE,{"models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl"}},
		{GLOVE_CT_SIDE,{"models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl"}},
		{GLOVE_SPORTY,{"models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"}},
		{GLOVE_SLICK,{"models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"}},
		{GLOVE_LEATHER_WRAP,{"models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"}},
		{GLOVE_MOTORCYCLE,{"models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"}},
		{GLOVE_SPECIALIST,{"models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"}},
		{GLOVE_HYDRA,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"}}
	};

	const auto entry = info.find(defindex);
	return entry == end(info) ? nullptr : &entry->second;
}

vector<game_data::weapon_name> game_data::model_names =
{
	{GLOVE_CT_SIDE, "CT Default" },
	{GLOVE_T_SIDE, "T Default" },
	{GLOVE_STUDDED_BLOODHOUND, "Bloodhound"},
	{GLOVE_SPORTY, "Sporty"},
	{GLOVE_SLICK, "Slick"},
	{GLOVE_LEATHER_WRAP, "Handwrap"},
	{GLOVE_MOTORCYCLE, "Motorcycle"},
	{GLOVE_SPECIALIST, "Specialist"},
	{GLOVE_HYDRA, "Hydra"},
	{WEAPON_KNIFE, "Default"},
	{WEAPON_KNIFE_BAYONET, "Bayonet"},
	{WEAPON_KNIFE_CSS, "Classic Knife"},
	{WEAPON_KNIFE_FLIP, "Flip Knife"},
	{WEAPON_KNIFE_GUT, "Gut Knife"},
	{WEAPON_KNIFE_KARAMBIT, "Karambit"},
	{WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet"},
	{WEAPON_KNIFE_TACTICAL, "Huntsman Knife"},
	{WEAPON_KNIFE_FALCHION, "Falchion Knife"},
	{WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife"},
	{WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife"},
	{WEAPON_KNIFE_PUSH, "Shadow Daggers"},
	{WEAPON_KNIFE_CORD, "Paracord Knife"},
	{WEAPON_KNIFE_CANIS, "Survival Knife"},
	{WEAPON_KNIFE_URSUS, "Ursus Knife"},
	{WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife"},
	{WEAPON_KNIFE_OUTDOOR, "Nomad Knife"},
	{WEAPON_KNIFE_STILETTO, "Stiletto Knife"},
	{WEAPON_KNIFE_WIDOWMAKER, "Talon Knife"},
	{WEAPON_KNIFE_SKELETON, "Skeleton Knife"},
	{GLOVE_STUDDED_BLOODHOUND, "Bloodhound"}
};

vector<game_data::weapon_name> game_data::knife_names =
{
	{WEAPON_KNIFE, "Default", 0},
	{WEAPON_KNIFE_BAYONET, "Bayonet", 1},
	{WEAPON_KNIFE_CSS, "Classic Knife", 2},
	{WEAPON_KNIFE_FLIP, "Flip Knife", 3},
	{WEAPON_KNIFE_GUT, "Gut Knife", 4},
	{WEAPON_KNIFE_KARAMBIT, "Karambit", 5},
	{WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet", 6},
	{WEAPON_KNIFE_TACTICAL, "Huntsman Knife", 7},
	{WEAPON_KNIFE_FALCHION, "Falchion Knife", 8},
	{WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife", 9},
	{WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife", 10},
	{WEAPON_KNIFE_PUSH, "Shadow Daggers", 11},
	{WEAPON_KNIFE_CORD, "Paracord Knife", 12},
	{WEAPON_KNIFE_CANIS, "Survival Knife", 13},
	{WEAPON_KNIFE_URSUS, "Ursus Knife", 14},
	{WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife", 15},
	{WEAPON_KNIFE_OUTDOOR, "Nomad Knife", 16},
	{WEAPON_KNIFE_STILETTO, "Stiletto Knife", 17},
	{WEAPON_KNIFE_WIDOWMAKER, "Talon Knife", 18},
	{WEAPON_KNIFE_SKELETON, "Skeleton Knife", 19}
};

vector<game_data::weapon_name> game_data::glove_names =
{ 
	{GLOVE_CT_SIDE, "CT Default" , 0},
	{GLOVE_T_SIDE, "T Default" , 1},
	{GLOVE_STUDDED_BLOODHOUND, "Bloodhound", 2},
	{GLOVE_SPORTY, "Sporty", 3},
	{GLOVE_SLICK, "Slick", 4},
	{GLOVE_LEATHER_WRAP, "Handwrap", 5},
	{GLOVE_MOTORCYCLE, "Motorcycle", 6},
	{GLOVE_SPECIALIST, "Specialist", 7},
	{GLOVE_HYDRA, "Hydra", 8}
};

vector<game_data::weapon_name> game_data::weap_names =
{   
	{ WEAPON_AK47, "AK47"  } ,
	{ WEAPON_M4A1, "M4A4"  },
	{ WEAPON_M4A1_SILENCER, "M4A1-S"},
	{ WEAPON_GALILAR, "Galil-AR"},
	{ WEAPON_FAMAS, "Famas"},
	{ WEAPON_AUG, "AUG" },
	{ WEAPON_SG556, "SG553",},
	{ WEAPON_AWP, "AWP" },
	{ WEAPON_SSG08, "SSG08" },
	{ WEAPON_G3SG1,  "G3SG1"},
	{ WEAPON_SCAR20,  "Scar-20" },
	{ WEAPON_GLOCK,  "Glock" },
	{ WEAPON_USP_SILENCER, "USP-S" },
	{ WEAPON_DEAGLE, "Deagle" },
	{ WEAPON_FIVESEVEN,  "Five-7" },
	{ WEAPON_ELITE,  "Dualies" },
	{ WEAPON_TEC9,  "Tec-9" },
	{ WEAPON_HKP2000, "P2000" },
	{ WEAPON_P250, "P250" },
	{ WEAPON_REVOLVER,  "R8-Revolver" },
	{ WEAPON_P90,  "P90" },
	{ WEAPON_UMP45,  "UMP-45" },
	{ WEAPON_BIZON,  "Bizon" },
	{ WEAPON_MAC10,  "Mac10" },
	{ WEAPON_MP7, "MP7" },
	{ WEAPON_MP9,  "MP9" },
	{ WEAPON_M249,  "M249" },
	{ WEAPON_XM1014,  "XM1014" },
	{ WEAPON_MAG7, "Mag-7" },
	{ WEAPON_NEGEV, "Negev" },
	{ WEAPON_SAWEDOFF, "Sawed Off" },
	{WEAPON_KNIFE_BAYONET, "Bayonet"},
	{WEAPON_KNIFE_FLIP, "Flip Knife"},
	{WEAPON_KNIFE_GUT, "Gut Knife"},
	{WEAPON_KNIFE_KARAMBIT, "Karambit"},
	{WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet"},
	{WEAPON_KNIFE_TACTICAL, "Huntsman Knife"},
	{WEAPON_KNIFE_FALCHION, "Falchion Knife"},
	{WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife"},
	{WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife"},
	{WEAPON_KNIFE_PUSH, "Shadow Daggers"},
	{WEAPON_KNIFE_CORD, "Paracord Knife"},
	{WEAPON_KNIFE_CANIS, "Survival Knife"},
	{WEAPON_KNIFE_URSUS, "Ursus Knife"},
	{WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife"},
	{WEAPON_KNIFE_OUTDOOR, "Nomad Knife"},
	{WEAPON_KNIFE_STILETTO, "Stiletto Knife"},
	{WEAPON_KNIFE_WIDOWMAKER, "Talon Knife"},
	{WEAPON_KNIFE_SKELETON, "Skeleton Knife"},	
	{GLOVE_STUDDED_BLOODHOUND, "Bloodhound"},
	{GLOVE_SPORTY, "Sporty"},
	{GLOVE_SLICK, "Slick"},
	{GLOVE_LEATHER_WRAP, "Handwrap"},
	{GLOVE_MOTORCYCLE, "Motorcycle"},
	{GLOVE_SPECIALIST, "Specialist"},
	{GLOVE_HYDRA, "Hydra"}
};

vector<game_data::quality_name> game_data::quality_names =
{
	{0, "Default"},
	{1, "Genuine"},
	{2, "Vintage"},
	{3, "Unusual"},
	{5, "Community"},
	{6, "Developer"},
	{7, "Self-Made"},
	{8, "Customized"},
	{9, "Strange"},
	{10, "Completed"},
	{12, "Tournament"}
};


const char* game_data::get_name(int id)
{
	switch (id)
	{
	case 1:return "deagle";
	case 2:return "elite";
	case 3:return "fiveseven";
	case 4:return "glock";
	case 7:return "ak47";
	case 8:return "aug";
	case 9:return "awp";
	case 10:return "famas";
	case 11:return "g3sg1";
	case 13:return "galilar";
	case 14:return "m249";
	case 60:return "m4a1_silencer";
	case 16:return "m4a1";
	case 17:return "mac10";
	case 19:return "p90";
	case 24:return "ump45";
	case 25:return "xm1014";
	case 26:return "bizon";
	case 27:return "mag7";
	case 28:return "negev";
	case 29:return "sawedoff";
	case 30:return "tec9";
	case 32:return "hkp2000";
	case 33:return "mp7";
	case 34:return "mp9";
	case 35:return "nova";
	case 36:return "p250";
	case 38:return "scar20";
	case 39:return "sg553";
	case 40:return "ssg08";
	case 61:return "usp_silencer";
	case 63:return "cz75a";
	case 64:return "revolver";
	case 508:return "knife_m9_bayonet";
	case 500:return "bayonet";
	case 505:return "knife_flip";
	case 506:return "knife_gut";
	case 507:return "knife_karambit";
	case 509:return "knife_tactical";
	case 512:return "knife_falchion";
	case 514:return "knife_survival_bowie";
	case 515:return "knife_butterfly";
	case 516:return "knife_push";
	case 517:return "knife_cord";
	case 518:return "knife_canis";
	case 519:return "knife_ursus";
	case 520:return "knife_gypsy_jackknife";
	case 521:return "knife_outdoor";
	case 522:return "knife_stiletto";
	case 523:return "knife_widowmaker";
	case 525:return "knife_skeleton";
	case 5027:return "studded_bloodhound_gloves";
	case 5030:return "sporty_gloves";
	case 5031:return "slick_gloves";
	case 5032:return "leather_handwraps";
	case 5033:return "motorcycle_gloves";
	case 5034:return "specialist_gloves";
	case 5035:return "studded_hydra_gloves";

	default:
		return "error";
	}
}

