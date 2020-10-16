#pragma once
#include "sdk.hpp"
#include "Hooks/recv_proxy_hook.hpp"

template <typename T>
auto get_entity_from_handle(sdk::CBaseHandle h) -> T*
{
	if (h == sdk::INVALID_EHANDLE_INDEX)
		return nullptr;

	return static_cast<T*>(g_entity_list->GetClientEntityFromHandle(h));
}

auto ensure_dynamic_hooks() -> void;
auto get_client_name() -> const char*;

extern recv_prop_hook* g_sequence_hook;