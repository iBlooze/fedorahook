#include <windows.h>
#include <stdio.h>
#include <iostream>

extern auto initialize(void* instance) -> void;

static auto WINAPI thread_entry(const LPVOID instance) -> DWORD
{
	initialize(instance);
	return 0;
}

auto WINAPI DllMain(const HINSTANCE instance, const DWORD reason, LPVOID reserved) -> bool
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		// Manual mappers sometimes pass nullptr here
		if(instance)
			DisableThreadLibraryCalls(instance);
		CreateThread(nullptr, 0, thread_entry, instance, 0, nullptr);
	}

	return true;
}
