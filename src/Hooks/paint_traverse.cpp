#include "hooks.hpp"
#include "../Features/visuals.hpp"
#include "../Utilities/config.hpp"
#include "../Utilities/drawing.hpp"

hooks::PaintTraverse::Fn* hooks::PaintTraverse::m_original;
void __fastcall hooks::PaintTraverse::hooked(sdk::IPanel* thisptr, int edx, unsigned int panel, bool repaint, bool force)
{
	static uint32_t hud_zoom_panel;
	static uint32_t focus_overlay_panel;

	// get zoom panel
	if (!strcmp("HudZoom", g_panel->GetName(panel)) && !hud_zoom_panel)
		hud_zoom_panel = panel;

	// add remove scope code if you want...
	// if (hud_zoom_panel == panel && g_local)

	m_original(thisptr, edx, panel, repaint, force);

	// setup focus overlay panel
	if (lstrcmpA(g_panel->GetName(panel), "MatSystemTopPanel") == 0 && !focus_overlay_panel)
	{
		drawing::setup_fonts();
		focus_overlay_panel = panel;
	}

	// here is where you draw shit
	if (focus_overlay_panel == panel)
		visuals::run_visuals();
}
