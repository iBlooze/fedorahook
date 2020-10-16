#pragma once
#include <cstdint>
#include "VMatrix.hpp"
class CClockDriftMgr
{
	friend class CBaseClientState;

public:
	CClockDriftMgr();

	// Is clock correction even enabled right now?
	static bool IsClockCorrectionEnabled();

	// Clear our state.
	void Clear();

	// This is called each time a server packet comes in. It is used to correlate
	// where the server is in time compared to us.
	void SetServerTick(int iServerTick);

	// Pass in the frametime you would use, and it will drift it towards the server clock.
	float AdjustFrameTime(float inputFrameTime);

	// Returns how many ticks ahead of the server the client is.
	float GetCurrentClockDifference() const;


private:

	void ShowDebugInfo(float flAdjustment);

	// This scales the offsets so the average produced is equal to the
	// current average + flAmount. This way, as we add corrections,
	// we lower the average accordingly so we don't keep responding
	// as much as we need to after we'd adjusted it a couple times.
	void AdjustAverageDifferenceBy(float flAmountInSeconds);


private:

	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float m_ClockOffsets[NUM_CLOCKDRIFT_SAMPLES];
	int m_iCurClockOffset;

	int m_nServerTick;		// Last-received tick from the server.
	int	m_nClientTick;		// The client's own tick counter (specifically, for interpolation during rendering).
							// The server may be on a slightly different tick and the client will drift towards it.
};

namespace sdk
{
	class CBaseClientState
	{
	public:
    public:
        void ForceFullUpdate()
        {
            m_nDeltaTick = -1;
        }

        char pad_0000[156];             //0x0000
        uint32_t m_NetChannel;          //0x009C
        uint32_t m_nChallengeNr;        //0x00A0
        char pad_00A4[100];             //0x00A4
        uint32_t m_nSignonState;        //0x0108
        char pad_010C[8];               //0x010C
        float m_flNextCmdTime;          //0x0114
        uint32_t m_nServerCount;        //0x0118
        uint32_t m_nCurrentSequence;    //0x011C
        char pad_0120[8];               //0x0120
        CClockDriftMgr m_ClockDriftMgr; //0x0128
        uint32_t m_nDeltaTick;          //0x0178
        bool m_bPaused;                 //0x017C
        char pad_017D[3];               //0x017D
        uint32_t m_nViewEntity;         //0x0180
        uint32_t m_nPlayerSlot;         //0x0184
        char m_szLevelName[260];        //0x0188
        char m_szLevelNameShort[40];    //0x028C
        char m_szGroupName[40];         //0x02B4
        char pad_02DC[56];              //0x02DC
        uint32_t m_nMaxClients;         //0x0310
        char pad_0314[18940];           //0x0314
        Vector viewangles;              //0x4D10
	};
}