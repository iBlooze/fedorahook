#pragma once
#include <string>
#include <vector>

extern auto run_update_check() -> void;

struct commit_entry
{
	std::string author;
	std::string date;
	std::string message;
};

extern bool g_update_needed;
extern std::vector<commit_entry> g_commits_since_compile;