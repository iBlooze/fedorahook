#include "config.hpp"
#include "../sdk.hpp"
#include <fstream>
#include <filesystem> // hack

nlohmann::json cfg;
using namespace game_data;
auto config::create_config_folder(std::string path) -> void
{
	if (!std::filesystem::create_directory(path)) return;
}

bool config::file_exists(std::string file)
{
	return std::filesystem::exists(file);
}

void config::save_config(const std::string path)
{
	std::ofstream output_file(path);

	if (!output_file.good())
		return;

	save(knife_model, "Knife Model");
	save(glove_model, "Glove Model");

	save(legit_enable, "Legitbot - enable");
	save_array(legit_fov, "Legitbot - fov");
	save_array(legit_smooth, "Legitbot - smooth");
	save_array(legit_rcs, "Legitbot - rcs value");
	save(legit_rcs_enable, "Legitbot - rcs");
	save(legit_triggerbot, "Legitbot - trigerbot");
	save_array(trigger_hitboxes, "Legitbot - trigger hitboxes");
	save(legit_silent_aim, "Legitbot - silent aim");
	save(legit_visible_check, "Legitbot - visible check");
	save(triggerbot_hitchance, "Legitbot - trigger hitchance");
	save(triggerbot_delay, "Legitbot - trigger delay");
	save_array(hitboxes, "Legitbot - hitboxes");
	save_array(legit_aim_at_backtrack, "Legitbot - aim at backtrack");

	save(legit_backtrack, "Backtrack - enable");
	save_array(legit_backtrack_ticks, "Backtrack - ticks");

	save(misc_bhop, "Bhop - enable");
	save(misc_bhop_hitchance, "Bhop - hitchance");
	save(misc_bhop_max_hits, "Bhop - max hits");
	save(misc_bhop_allowed_misses, "Bhop - allowed misses");

	save(chams_enable, "Visuals - chams");
	save(chams_enable_behind, "Visuals - enable nonvisible");

	save_array(menu_color, "Color - menu");
	save_array(chams_vis_color, "Color - visible chams");
	save_array(chams_invis_color, "Color - invisible chams");

	for (int id = WEAPON_DEAGLE; id <= WEAPON_REVOLVER; id++)  // Weapons
	{
		std::string skin = get_name(id), stat_track = get_name(id), seed = get_name(id), wear = get_name(id);
		skin += ": skin";
		seed += ": seed";
		wear += ": wear";
		stat_track += ": stat_track";

		if (get_name(id) == "error")
			continue;

		save(w_settings[id].skin, skin);
		save(w_settings[id].seed, seed);
		save(w_settings[id].wear, wear);
		save(w_settings[id].stat_track, stat_track);
	}

	for (int id = WEAPON_KNIFE_BAYONET; id <= WEAPON_KNIFE_SKELETON; id++)  // Knives
	{
		std::string skin = get_name(id), stat_track = get_name(id), seed = get_name(id), wear = get_name(id);
		skin += ": skin";
		seed += ": seed";
		wear += ": wear";
		stat_track += ": stat_track";

		if (get_name(id) == "error")
			continue;

		save(w_settings[id].skin, skin);
		save(w_settings[id].seed, seed);
		save(w_settings[id].wear, wear);
		save(w_settings[id].stat_track, stat_track);
	}

	for (int id = GLOVE_STUDDED_BLOODHOUND; id <= GLOVE_HYDRA; id++)  // Gloves
	{
		std::string skin = get_name(id), stat_track = get_name(id), seed = get_name(id), wear = get_name(id);
		skin += ": skin";
		seed += ": seed";
		wear += ": wear";
		stat_track += ": stat_track";

		if (get_name(id) == "error")
			continue;

		save(w_settings[id].skin, skin);
		save(w_settings[id].seed, seed);
		save(w_settings[id].wear, wear);
		save(w_settings[id].stat_track, stat_track);
	}

	output_file << std::setw(4) << cfg << std::endl;
	output_file.close();
}

void config::load_config(const std::string path)
{
	std::ifstream input_file(path);

	if (!input_file.good())
		return;

	try
	{
		cfg << input_file;
	}
	catch (...)
	{
		input_file.close();
		return;
	}

	load(knife_model, "Knife Model");
	load(glove_model, "Glove Model");

	load(legit_enable, "Legitbot - enable");
	load_array(legit_fov, "Legitbot - fov");
	load_array(legit_smooth, "Legitbot - smooth");
	load_array(legit_rcs, "Legitbot - rcs value");
	load(legit_rcs_enable, "Legitbot - rcs");
	load(legit_triggerbot, "Legitbot - trigerbot");
	load_array(trigger_hitboxes, "Legitbot - trigger hitboxes");
	load(legit_silent_aim, "Legitbot - silent aim");
	load(legit_visible_check, "Legitbot - visible check");
	load(triggerbot_hitchance, "Legitbot - trigger hitchance");
	load(triggerbot_delay, "Legitbot - trigger delay");
	load_array(hitboxes, "Legitbot - hitboxes");
	load_array(legit_aim_at_backtrack, "Legitbot - aim at backtrack");

	load(legit_backtrack, "Backtrack - enable");
	load_array(legit_backtrack_ticks, "Backtrack - ticks");

	load(misc_bhop, "Bhop - enable");
	load(misc_bhop_hitchance, "Bhop - hitchance");
	load(misc_bhop_max_hits, "Bhop - max hits");
	load(misc_bhop_allowed_misses, "Bhop - allowed misses");

	load(chams_enable, "Visuals - chams");
	load(chams_enable_behind, "Visuals - enable nonvisible");

	load_array(menu_color, "Color - menu");
	load_array(chams_vis_color, "Color - visible chams");
	load_array(chams_invis_color, "Color - invisible chams");

	for (int id = WEAPON_DEAGLE; id <= WEAPON_REVOLVER; id++)  // Weapons
	{
		std::string skin = get_name(id), stat_track = get_name(id), seed = get_name(id), wear = get_name(id);
		skin += ": skin";
		seed += ": seed";
		wear += ": wear";
		stat_track += ": stat_track";

		if (get_name(id) == "error")
			continue;

		load(w_settings[id].skin, skin);
		load(w_settings[id].seed, seed);
		load(w_settings[id].wear, wear);
		load(w_settings[id].stat_track, stat_track);
	}

	for (int id = WEAPON_KNIFE_BAYONET; id <= WEAPON_KNIFE_SKELETON; id++)  // Knives
	{
		std::string skin = get_name(id), stat_track = get_name(id), seed = get_name(id), wear = get_name(id);
		skin += ": skin";
		seed += ": seed";
		wear += ": wear";
		stat_track += ": stat_track";

		if (get_name(id) == "error")
			continue;

		load(w_settings[id].skin, skin);
		load(w_settings[id].seed, seed);
		load(w_settings[id].wear, wear);
		load(w_settings[id].stat_track, stat_track);
	}

	for (int id = GLOVE_STUDDED_BLOODHOUND; id <= GLOVE_HYDRA; id++)  // Gloves
	{
		std::string skin = get_name(id), stat_track = get_name(id), seed = get_name(id), wear = get_name(id);
		skin += ": skin";
		seed += ": seed";
		wear += ": wear";
		stat_track += ": stat_track";

		if (get_name(id) == "error")
			continue;

		load(w_settings[id].skin, skin);
		load(w_settings[id].seed, seed);
		load(w_settings[id].wear, wear);
		load(w_settings[id].stat_track, stat_track);
	}

	input_file.close();
}

template<typename T>
void config::load(T& value, std::string str)
{
	if (cfg[str].empty())
		return;

	value = cfg[str].get<T>();
}

void config::load_array(float value[5], std::string str)
{
	if (cfg[str].empty())
		return;

	value[0] = cfg[str]["0"].get<float>();
	value[1] = cfg[str]["1"].get<float>();
	value[2] = cfg[str]["2"].get<float>();
	value[3] = cfg[str]["3"].get<float>();
	value[4] = cfg[str]["4"].get<float>();
}

void config::load_array(int value[5], std::string str)
{
	if (cfg[str].empty())
		return;

	value[0] = cfg[str]["0"].get<int>();
	value[1] = cfg[str]["1"].get<int>();
	value[2] = cfg[str]["2"].get<int>();
	value[3] = cfg[str]["3"].get<int>();
	value[4] = cfg[str]["4"].get<int>();
}

void config::load_array(bool value[5], std::string str)
{
	if (cfg[str].empty())
		return;

	value[0] = cfg[str]["0"].get<bool>();
	value[1] = cfg[str]["1"].get<bool>();
	value[2] = cfg[str]["2"].get<bool>();
	value[3] = cfg[str]["3"].get<bool>();
	value[4] = cfg[str]["4"].get<bool>();
}

void config::load_array(bool value[5][3], std::string str)
{
	if (cfg[str].empty())
		return;

	value[0][0] = cfg[str]["0.0"].get<bool>();
	value[1][0] = cfg[str]["0.1"].get<bool>();
	value[2][0] = cfg[str]["0.2"].get<bool>();
	value[3][0] = cfg[str]["0.3"].get<bool>();
	value[4][0] = cfg[str]["0.4"].get<bool>();

	value[0][1] = cfg[str]["1.0"].get<bool>();
	value[1][1] = cfg[str]["1.1"].get<bool>();
	value[2][1] = cfg[str]["1.2"].get<bool>();
	value[3][1] = cfg[str]["1.3"].get<bool>();
	value[4][1] = cfg[str]["1.4"].get<bool>();

	value[0][2] = cfg[str]["2.0"].get<bool>();
	value[1][2] = cfg[str]["2.1"].get<bool>();
	value[2][2] = cfg[str]["2.2"].get<bool>();
	value[3][2] = cfg[str]["2.3"].get<bool>();
	value[4][2] = cfg[str]["2.4"].get<bool>();
}

template<typename T>
void config::save(T& value, std::string str)
{
	cfg[str] = value;
}

void config::save_array(float value[5], std::string str)
{
	cfg[str]["0"] = value[0];
	cfg[str]["1"] = value[1];
	cfg[str]["2"] = value[2];
	cfg[str]["3"] = value[3];
	cfg[str]["4"] = value[4];
}

void config::save_array(int value[5], std::string str)
{
	cfg[str]["0"] = value[0];
	cfg[str]["1"] = value[1];
	cfg[str]["2"] = value[2];
	cfg[str]["3"] = value[3];
	cfg[str]["4"] = value[4];
}

void config::save_array(bool value[5], std::string str)
{
	cfg[str]["0"] = value[0];
	cfg[str]["1"] = value[1];
	cfg[str]["2"] = value[2];
	cfg[str]["3"] = value[3];
	cfg[str]["4"] = value[4];
}

void config::save_array(bool value[5][3], std::string str)
{
	cfg[str]["0.0"] = value[0][0];
	cfg[str]["0.1"] = value[1][0];
	cfg[str]["0.2"] = value[2][0];
	cfg[str]["0.3"] = value[3][0];
	cfg[str]["0.4"] = value[4][0];

	cfg[str]["1.0"] = value[0][1];
	cfg[str]["1.1"] = value[1][1];
	cfg[str]["1.2"] = value[2][1];
	cfg[str]["1.3"] = value[3][1];
	cfg[str]["1.4"] = value[4][1];

	cfg[str]["2.0"] = value[0][2];
	cfg[str]["2.1"] = value[1][2];
	cfg[str]["2.2"] = value[2][2];
	cfg[str]["2.3"] = value[3][2];
	cfg[str]["2.4"] = value[4][2];
}

std::vector<std::string> config::get_configs()
{
	std::vector<config_file> config_files = get_configs_in_folder(config_path, ".brood");
	std::vector<std::string> config_file_names;

	for (auto config = config_files.begin(); config != config_files.end(); config++)
		config_file_names.emplace_back(config->get_name());

	std::sort(config_file_names.begin(), config_file_names.end());

	return config_file_names;
}

std::vector<config_file> config::get_configs_in_folder(const std::string path, const std::string ext)
{
	namespace fs = std::filesystem;
	bool exists = fs::exists(path), directory = fs::is_directory(path);
	std::vector<config_file> config_files;

	if (exists && directory)
	{
		for (auto it = fs::recursive_directory_iterator(path); it != fs::recursive_directory_iterator(); it++)
		{
			if (fs::is_regular_file(*it) && it->path().extension() == ext)
			{
				std::string f_path = path + it->path().filename().string();

				std::string tmp_f_name = it->path().filename().string();
				size_t pos = tmp_f_name.find(".");
				std::string f_name = (std::string::npos == pos) ? tmp_f_name : tmp_f_name.substr(0, pos);

				config_file new_config(f_name, f_path);
				config_files.emplace_back(new_config);
			}
		}
	}
	return config_files;
}