#pragma once
#include <cstddef>
#include <array>

template <typename Fn = void*>
Fn get_vfunc(void* class_base, const std::size_t index)
{
	return (*reinterpret_cast<Fn**>(class_base))[index];
}

namespace sdk
{
	class C_BaseEntity;
	class ClientClass;
	class ClientClass;
	class IClientAlphaProperty;
	class IClientEntity;
	class IClientEntity;
	class IClientNetworkable;
	class IClientNetworkable;
	class IClientRenderable;
	class IClientThinkable;
	class IClientUnknown;
	class ICollideable;
	class IGameEvent;
	class KeyValues;
	class RecvProp;
	class RecvTable;

	struct AppSystemInfo_t;
	struct CRecvProxyData;
	struct bf_read;
	struct bf_read;
	struct bf_write;
	struct model_t;

	using CBaseHandle = unsigned long;
	using CreateClientClassFn = IClientNetworkable* (*)(int, int);
	using CreateEventFn = IClientNetworkable* (*)();
	using CreateInterfaceFn = void* (*)(const char*, int*);
	using LocalizeStringIndex_t = unsigned;
	using RecvVarProxyFn = void(__cdecl *)(const CRecvProxyData*, void*, void*);

	constexpr static auto INVALID_EHANDLE_INDEX = 0xFFFFFFFF;
	constexpr static auto MAX_PLAYER_NAME_LENGTH = 128;
	constexpr static auto SIGNED_GUID_LEN = 32;
	constexpr static auto MAX_WEAPONS = 48;
	constexpr static auto MAX_PLAYERS = 65; // Only cstrike and csgo

	enum class LifeState
	{
		ALIVE = 0,
		DYING,
		DEAD,
		RESPAWNABLE,
		DISCARDBODY,
	};

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,	// never moves
		MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
		MOVETYPE_WALK,				// Player only - moving on the ground
		MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
		MOVETYPE_FLY,				// No gravity, but still collides with stuff
		MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
		MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
		MOVETYPE_PUSH,				// no clip to world, push and crush
		MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
		MOVETYPE_LADDER,			// Used by players only when going onto a ladder
		MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
		MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

		// should always be defined as the last item in the list
		MOVETYPE_LAST = MOVETYPE_CUSTOM,

		MOVETYPE_MAX_BITS = 4
	};

	enum EntityFlags
	{
		FL_ONGROUND = (1 << 0),
		FL_DUCKING = (1 << 1),
		FL_WATERJUMP = (1 << 2),
		FL_ONTRAIN = (1 << 3),
		FL_INRAIN = (1 << 4),
		FL_FROZEN = (1 << 5),
		FL_ATCONTROLS = (1 << 6),
		FL_CLIENT = (1 << 7),
		FL_FAKECLIENT = (1 << 8),
		MAX_ENTITYFLAGS
	};

	enum class ClientFrameStage_t
	{
		FRAME_UNDEFINED = -1,
		FRAME_START,
		FRAME_NET_UPDATE_START,
		FRAME_NET_UPDATE_POSTDATAUPDATE_START,
		FRAME_NET_UPDATE_POSTDATAUPDATE_END,
		FRAME_NET_UPDATE_END,
		FRAME_RENDER_START,
		FRAME_RENDER_END
	};
}