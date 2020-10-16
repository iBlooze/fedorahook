#include "drawing.hpp"

unsigned long drawing::font;
auto drawing::setup_fonts() -> void
{
	drawing::font = g_surface->CreateFont_();
	g_surface->SetFontGlyphSet(drawing::font, "Small Fonts", 9, 400, 0, 0, sdk::FONTFLAG_OUTLINE);
}

auto drawing::render_text(int x, int y, Color clr, const char* fmt, ...) -> void
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	g_surface->DrawSetTextPos(x, y);
	g_surface->DrawSetTextFont(drawing::font);
	g_surface->DrawSetTextColor(clr);
	g_surface->DrawPrintText(wbuf, wcslen(wbuf));
}