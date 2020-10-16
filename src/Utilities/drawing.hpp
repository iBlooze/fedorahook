#pragma once
#include "../sdk.hpp"
#include "../SDK/Colors.hpp"

namespace drawing
{
	auto setup_fonts() -> void;
	auto render_text(int x, int y, Color clr, const char* fmt, ...) -> void;
	extern unsigned long font;
}
