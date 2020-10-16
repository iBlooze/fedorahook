#pragma once

#include <memory>
#include "../SDK.hpp"

namespace event_listener
{
	void initialize_events();

	class global_listener : public sdk::IGameEventListener 
	{
	public:
		~global_listener() { g_game_event_manager->RemoveListener(this); }
		virtual void FireGameEvent(sdk::IGameEvent* game_event);
	};
};
