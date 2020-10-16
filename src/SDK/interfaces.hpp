#pragma once
#include "CBaseClientState.hpp"
#include "IBaseClientDLL.hpp"
#include "IClientEntityList.hpp"
#include "IGameEventManager.hpp"
#include "ILocalize.hpp"
#include "IVEngineClient.hpp"
#include "IVModelInfoClient.hpp"
#include "IInputSystem.hpp"
#include "ISurface.hpp"
#include "IVRenderView.hpp"
#include "IMaterialSystem.hpp"
#include "IVModelRender.hpp"
#include "IClientMode.hpp"
#include "CBaseGlobalVars.hpp"
#include "CInput.hpp"

#define CLIENT_DLL_INTERFACE_VERSION		"VClient018"
extern sdk::IBaseClientDLL*					g_client;

#define VGUIMAT_SURFACE_INTERFACE_VERSION		"VGUI_Surface031"
extern sdk::ISurface*						g_surface;

#define VCLIENTENTITYLIST_INTERFACE_VERSION "VClientEntityList003"
extern sdk::IClientEntityList*				g_entity_list;

#define VENGINE_CLIENT_INTERFACE_VERSION	"VEngineClient014"
extern sdk::IVEngineClient*					g_engine;

#define VENGINE_FILE_INTERFACE_VERSION	    "VFileSystem017"
extern sdk::IFileSystem*					g_FileSystem;

#define VMODELINFO_CLIENT_INTERFACE_VERSION "VModelInfoClient004"
extern sdk::IVModelInfoClient*				g_model_info;

#define INTERFACEVERSION_GAMEEVENTSMANAGER2	"GAMEEVENTSMANAGER002"
extern sdk::IGameEventManager*				g_game_event_manager;

#define ILOCALIZE_CLIENT_INTERFACE_VERSION	"Localize_001"
extern sdk::ILocalize*						g_localize;

#define INPUTSYSTEM_INTERFACE_VERSION		"InputSystemVersion001"
extern sdk::IInputSystem*					g_input_system;

#define RENDERMODEL_INTERFACE_VERSION		"VEngineModel016"
extern sdk::IVModelRender*					g_model_render;

#define VRENDERVIEW_INTERFACE_VERSION		"VEngineRenderView014"
extern sdk::IVRenderView*					g_render_view;

#define VMATERIAL_SYSTEM_INTERFACE_VERSION	"VMaterialSystem080"
extern sdk::IVMaterialSystem* g_material;

#define VENGINE_TRACE_INTERFACE_VERSION		"EngineTraceClient004"
extern sdk::IEngineTrace*					g_engine_trace;

#define VGUI_PANEL_VERSION				"VGUI_Panel009"
extern sdk::IPanel* g_panel;

#define VENGINE_PREDICTION_VERSION			"VClientPrediction001"
extern sdk::IPrediction* g_prediction;

#define VSTD_CVAR_VERSION				"VEngineCvar007"
extern sdk::ICvar* g_cvar;

#define VENGINE_SOUND_VERSION				"IEngineSoundClient003"
extern sdk::IEngineSound* g_sound;

#define VENGINE_MOVEMENT_VERSION			"GameMovement001"
extern sdk::IGameMovement* g_movement;

#define NETWORK_STRING_CONTAINER_VERSION	"VEngineClientStringTable001"
extern sdk::INetworkStringTableContainer* g_network_string;

extern sdk::C_BasePlayer*					g_local;
extern sdk::CGlobalVarsBase*				g_global_vars;
extern sdk::IClientMode*					g_client_mode;
extern sdk::CBaseClientState**				g_client_state;
extern sdk::C_CS_PlayerResource**			g_player_resource;
