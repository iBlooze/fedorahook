#include "hooks.hpp"
#include "../Utilities/item_definitions.hpp"
#include "../main.hpp"
#include "../Utilities/config.hpp"
#include "../Utilities/kit_parser.hpp"
#include "../SDK/VMatrix.hpp"

hooks::FrameStageNotify::Fn* hooks::FrameStageNotify::m_original;
void __stdcall hooks::FrameStageNotify::hooked(sdk::ClientFrameStage_t stage)
{
	if (!g_local || !g_engine->IsInGame() || !g_engine->IsConnected())
		return m_original(stage);

	switch (stage)
	{
	case sdk::ClientFrameStage_t::FRAME_START:
		break;

	case sdk::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		break;

	case sdk::ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		break;

	case sdk::ClientFrameStage_t::FRAME_NET_UPDATE_END:
		break;

	case sdk::ClientFrameStage_t::FRAME_RENDER_START:
		break;

	case sdk::ClientFrameStage_t::FRAME_RENDER_END:
		break;
	}

	return m_original(stage);
}
