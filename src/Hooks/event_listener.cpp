#include "event_listener.hpp"

event_listener::global_listener global_event_listener;

void event_listener::initialize_events()
{
	g_game_event_manager->AddListenerGlobal(&global_event_listener, false);
}

void event_listener::global_listener::FireGameEvent(sdk::IGameEvent* event)
{
	if (!event)
		return;

	switch (fnv::hash_runtime(event->GetName()))
	{
	// clear data
	case FNV("round_start"): 
		break;

	// for logging purchases
	case FNV("item_purchase"): 
		break;

	// for hitmarker and other things
	case FNV("player_hurt"): 
		break;

	// logging kills / deaths
	case FNV("player_death"): 
		break;

	// bullet tracer / other things
	case FNV("bullet_impact"): 
		break;
	}
}
