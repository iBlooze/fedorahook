#pragma once
#include "../sdk.hpp"
class IMatRenderContext;
struct ModelRenderInfo_t;
struct DrawModelState_t;

namespace hooks
{
	struct CCSPlayer_PostDataUpdate
	{
		using Fn = void __fastcall(sdk::IClientNetworkable* thisptr, void*, int update_type);
		static Fn hooked;
		static Fn* m_original;
	};

	struct LockCursor
	{
		using Fn = void __fastcall(sdk::ISurface* thisptr, int edx);
		static Fn hooked;
		static Fn* m_original;
	};

	struct DrawModelExecute
	{
		using Fn = void __fastcall(void* _this, int, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
		static Fn hooked;
		static Fn* m_original;
	};

	struct CreateMove
	{
		using Fn = bool __fastcall(sdk::IClientMode*, void*, float, sdk::CUserCmd*);
		static Fn hooked;
		static Fn* m_original;
	};

	struct PaintTraverse
	{
		using Fn = void __fastcall(sdk::IPanel*, int, unsigned int, bool, bool);
		static Fn hooked;
		static Fn* m_original;
	};

	struct FireEventClientSide
	{
		using Fn = bool __fastcall(void*, sdk::IGameEvent*);
		static Fn hooked;
		static Fn* m_original;
	};

	struct FrameStageNotify
	{
		using Fn = void __stdcall(sdk::ClientFrameStage_t stage);
		static Fn hooked;
		static Fn* m_original;
	};

	// other hook tooks
	int parse_skins();
	bool valid_entity(sdk::C_BasePlayer* ent);
	extern auto __cdecl sequence_proxy_fn(const sdk::CRecvProxyData* proxy_data_const, void* entity, void* output) -> void;
}
