#include "sdk.hpp"
#include "shlobj.h"
#include "windows.h"
#include "main.hpp"
#include "Hooks/hooks.hpp"
#include "Utilities/render.hpp"
#include "Utilities/config.hpp"
#include "Utilities/update_check.hpp"
#include "Hooks/event_listener.hpp"

sdk::ISurface* g_surface;
sdk::IBaseClientDLL* g_client;
sdk::IClientEntityList* g_entity_list;
sdk::IVEngineClient* g_engine;
sdk::IVModelInfoClient* g_model_info;
sdk::IGameEventManager* g_game_event_manager;
sdk::ILocalize* g_localize;
sdk::IInputSystem* g_input_system;
sdk::IFileSystem* g_FileSystem;
sdk::CBaseClientState** g_client_state;
sdk::C_CS_PlayerResource** g_player_resource;
sdk::IVRenderView* g_render_view;
sdk::IVModelRender* g_model_render;
sdk::IVMaterialSystem* g_material;
sdk::IClientMode* g_client_mode;
sdk::CGlobalVarsBase* g_global_vars;
sdk::C_BasePlayer* g_local;
sdk::IEngineTrace* g_engine_trace;
sdk::IPanel* g_panel;
sdk::IPrediction* g_prediction;
sdk::ICvar* g_cvar;
sdk::IEngineSound* g_sound;
sdk::IGameMovement* g_movement;
sdk::INetworkStringTableContainer* g_network_string;

//vmt_smart_hook*				g_client_hook;
//vmt_smart_hook*				g_game_event_manager_hook;

recv_prop_hook* g_sequence_hook;

auto ensure_dynamic_hooks() -> void
{
	const auto local_index = g_engine->GetLocalPlayer();
	const auto local = static_cast<sdk::C_BasePlayer*>(g_entity_list->GetClientEntity(local_index));
	if (local)
	{
		static vmt_multi_hook player_hook;
		const auto networkable = static_cast<sdk::IClientNetworkable*>(local);
		if (player_hook.initialize_and_hook_instance(networkable))
			player_hook.apply_hook<hooks::CCSPlayer_PostDataUpdate>(7);
	}
}

template <class T>
auto get_interface(const char* module, const char* name) -> T*
{
	return reinterpret_cast<T*>(platform::get_interface(module, name));
}

auto get_client_name() -> const char*
{
	static const char* name = nullptr;
	if (!name)
		name = platform::get_module_info("client_panorama.dll").first ? "client_panorama.dll" : "client.dll";
	return name;
}

void __fastcall hooks::LockCursor::hooked(sdk::ISurface* thisptr, int edx)
{
	if (render::s_active)
	{
		g_surface->UnlockCursor();
		return;
	}

	m_original(thisptr, edx);
}

hooks::LockCursor::Fn* hooks::LockCursor::m_original;

auto initialize(void* instance) -> void
{
	// config tings
	long res = SHGetFolderPathA(0, 0x0005, 0, 0, game_data::config_folder);
	game_data::config_path = std::string(game_data::config_folder) + "\\skins\\";

	// get client interfaces
	g_prediction = get_interface<sdk::IPrediction>("client.dll", VENGINE_PREDICTION_VERSION);
	g_movement = get_interface<sdk::IGameMovement>("client.dll", VENGINE_MOVEMENT_VERSION);
	g_client = get_interface<sdk::IBaseClientDLL>("client.dll", CLIENT_DLL_INTERFACE_VERSION);
	g_entity_list = get_interface<sdk::IClientEntityList>("client.dll", VCLIENTENTITYLIST_INTERFACE_VERSION);

	// get engine interfaces
	g_sound = get_interface<sdk::IEngineSound>("engine.dll", VENGINE_SOUND_VERSION);
	g_engine = get_interface<sdk::IVEngineClient>("engine.dll", VENGINE_CLIENT_INTERFACE_VERSION);
	g_render_view = get_interface<sdk::IVRenderView>("engine.dll", VRENDERVIEW_INTERFACE_VERSION);
	g_model_render = get_interface<sdk::IVModelRender>("engine.dll", RENDERMODEL_INTERFACE_VERSION);
	g_engine_trace = get_interface<sdk::IEngineTrace>("engine.dll", VENGINE_TRACE_INTERFACE_VERSION);
	g_model_info = get_interface<sdk::IVModelInfoClient>("engine.dll", VMODELINFO_CLIENT_INTERFACE_VERSION);
	g_game_event_manager = get_interface<sdk::IGameEventManager>("engine.dll", INTERFACEVERSION_GAMEEVENTSMANAGER2);
	g_network_string = get_interface<sdk::INetworkStringTableContainer>("engine.dll", NETWORK_STRING_CONTAINER_VERSION);

	// get other interfaces
	g_panel = get_interface<sdk::IPanel>("vgui2.dll", VGUI_PANEL_VERSION);
	g_cvar = get_interface<sdk::ICvar>("vstdlib.dll", VSTD_CVAR_VERSION);
	g_surface = get_interface<sdk::ISurface>("vguimatsurface.dll", VGUIMAT_SURFACE_INTERFACE_VERSION);
	g_localize = get_interface<sdk::ILocalize>("localize.dll", ILOCALIZE_CLIENT_INTERFACE_VERSION);
	g_input_system = get_interface<sdk::IInputSystem>("inputsystem.dll", INPUTSYSTEM_INTERFACE_VERSION);
	g_material = get_interface<sdk::IVMaterialSystem>("materialsystem.dll", VMATERIAL_SYSTEM_INTERFACE_VERSION);
	g_client_mode = **reinterpret_cast<sdk::IClientMode***>((*reinterpret_cast<uintptr_t**>(g_client))[10] + 5);
	g_global_vars = **reinterpret_cast<sdk::CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_client))[11] + 10);
	g_client_state = *reinterpret_cast<sdk::CBaseClientState***>(get_vfunc<std::uintptr_t>(g_engine, 12) + 0x10);

	// setup events.
	event_listener::initialize_events();

	// check for update
	run_update_check();

	// create config folder if doesnt exist
	if (res == S_OK)
		config::create_config_folder(game_data::config_folder);

	// load and sort skins
	hooks::parse_skins();

	// setup rendering for menu
	render::initialize();

	// hook everything
	static vmt_multi_hook client_mode_hook, client_hook, 
		surfacedll_hook, model_render_hook, panel_hook;

	if (client_mode_hook.initialize_and_hook_instance(g_client_mode))
		client_mode_hook.apply_hook<hooks::CreateMove>(24);

	if (surfacedll_hook.initialize_and_hook_instance(g_surface))
		surfacedll_hook.apply_hook<hooks::LockCursor>(67);

	if (model_render_hook.initialize_and_hook_instance(g_model_render))
		model_render_hook.apply_hook<hooks::DrawModelExecute>(21);

	if (panel_hook.initialize_and_hook_instance(g_panel))
		panel_hook.apply_hook<hooks::PaintTraverse>(41);

	if (client_hook.initialize_and_hook_instance(g_client))
		client_hook.apply_hook<hooks::FrameStageNotify>(37);

	const auto sequence_prop = sdk::C_BaseViewModel::GetSequenceProp();
	g_sequence_hook = new recv_prop_hook(sequence_prop, &hooks::sequence_proxy_fn);
}

// If we aren't unloaded correctly (like when you close csgo)
// we should just leak the hooks, since the hooked instances
// might be already destroyed
auto uninitialize() -> void
{
	render::uninitialize();

	//delete g_client_hook;
	//delete g_game_event_manager_hook;

	delete g_sequence_hook;
}
