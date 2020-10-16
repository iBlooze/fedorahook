#include "item_definitions.hpp"
#include <fstream>
#include <json.hpp>
#include <filesystem> // hack

class config_file;

namespace config
{
	void create_config_folder(std::string path);
	bool file_exists(std::string file);

	void save_config(const std::string path);
	void load_config(const std::string path);

	void load_array(float value[5], std::string str);

	void load_array(int value[5], std::string str);

	void load_array(bool value[5], std::string str);

	void load_array(bool value[5][3], std::string str);

	void save_array(float value[5], std::string str);

	void save_array(int value[5], std::string str);

	void save_array(bool value[5], std::string str);

	void save_array(bool value[5][3], std::string str);

	std::vector<std::string> get_configs();

	std::vector<config_file> get_configs_in_folder(const std::string path, const std::string ext);

	template<typename T>
	void load(T& value, std::string str);

	template<typename T>
	void save(T& value, std::string str);

}; 

class config_file
{

public:

	config_file(const std::string name, const std::string path)
	{
		this->name = name;
		this->path = path;
	}

	std::string get_name() { return name; };
	std::string get_path() { return path; };

private:

	std::string name, path;
};
