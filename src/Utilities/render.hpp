#pragma once

#include <imgui.h>

// Other parts don't need to know what goes behind the scenes
namespace render
{
	extern void initialize();
	extern void uninitialize();

	extern bool s_active;

	extern ImFont* font_main;
	extern ImFont* font_menu;
}