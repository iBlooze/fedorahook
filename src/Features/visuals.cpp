#include "visuals.hpp"
#include "legitbot.hpp"
#include "../Utilities/drawing.hpp"

auto visuals::run_visuals() -> void
{
	drawing::render_text(100, 100, Color(255, 0, 0), "TEST STRING");

	if (!g_engine->IsInGame())
		return;

	for (int it = 1; it <= 64; it++)
	{
		auto cur_ent = static_cast<sdk::C_BasePlayer*>(g_entity_list->GetClientEntity(it));
		if (!cur_ent || !hooks::valid_entity(cur_ent))
			continue;

		// apply visuals to every player.
		visualize_player(cur_ent);
	}
}


auto visuals::visualize_player(sdk::C_BasePlayer* ent) -> void
{

}