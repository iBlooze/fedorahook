#pragma once
#include "declarations.hpp"
#include "IClientEntity.hpp"
#include "../Utilities/netvar_manager.hpp"
#include "UtlVector.hpp"

namespace sdk
{
	class CCSWeapData
	{
	public:
		char pad_0x0000[0x4]; //0x0000
		char* m_szWeaponName; //0x0004 
		char pad_0x0008[0xC]; //0x0008
		__int32 m_iMaxClip; //0x0014 
		char pad_0x0018[0xC]; //0x0018
		__int32 m_iMaxReservedAmmo; //0x0024 
		char pad_0x0028[0x4]; //0x0028
		char* m_szWeaponMdlPath; //0x002C 
		char* m_szWeaponMdlPath2; //0x0030 
		char* m_szDroppedWeaponMdlPath; //0x0034 
		char* m_szDefaultClip; //0x0038 
		char pad_0x003C[0x44]; //0x003C
		char* m_szBulletType; //0x0080		
		char pad_0x0084[0x4]; //0x0084
		char* m_szHudName; //0x0088 
		char* m_szWeaponName2; //0x008C 
		char pad_0x0090[0x38]; //0x0090
		__int32 m_iWeaponType; //0x00C8 
		__int32 m_iWeaponType2; //0x00CC 
		__int32 m_iWeaponPrice; //0x00D0 
		__int32 m_iWeaponReward; //0x00D4 
		char* m_szWeaponType; //0x00D8 
		float m_flUnknownFloat0; //0x00DC 
		char pad_0x00E0[0xC]; //0x00E0
		unsigned char m_ucFullAuto; //0x00EC 
		char pad_0x00ED[0x3]; //0x00ED
		__int32 m_iWeaponDamage; //0x00F0 
		float m_flArmorRatio; //0x00F4 
		__int32 m_iBullets; //0x00F8 
		float m_flPenetration; //0x00FC 
		char pad_0x0100[0x8]; //0x0100
		float m_flWeaponRange; //0x0108 
		float m_flRangeModifier; //0x010C 
		float m_flThrowVelocity; //0x0110 
		char pad_0x0114[0xC]; //0x0114
		unsigned char m_ucHasSilencer; //0x0120 
		char pad_0x0121[0xF]; //0x0121
		float m_flMaxSpeed; //0x0130 
		float m_flMaxSpeed2; //0x0134 
		float m_flAttackMoveSpeedFactor; //0x0138
		float m_flSpread; //0x013C
		float m_flSpreadAlt; //0x0140
		float m_flInaccuracyCrouch; //0x0144
		float m_flInaccuracyCrouchAlt; //0x0148
		float m_flInaccuracyStand; //0x014C
		float m_flInaccuracyStandAlt; //0x0150
		float m_flInaccuracyJump; //0x0154
		float m_flInaccuracyJumpAlt; //0x0158
		char pad_0x015C[0x28]; //0x015C
		__int32 m_iRecoilSeed; //0x0184 
		char pad_0x0188[0x68]; //0x0188
		char* m_szWeaponTracesType; //0x01F0 
		char pad_0x01F4[0x648]; //0x01F4


	}; //Size=0x082C

	enum CSWeaponType
	{
		WEAPONTYPE_KNIFE = 0,
		WEAPONTYPE_PISTOL,
		WEAPONTYPE_SUBMACHINEGUN,
		WEAPONTYPE_RIFLE,
		WEAPONTYPE_SHOTGUN,
		WEAPONTYPE_SNIPER_RIFLE,
		WEAPONTYPE_MACHINEGUN,
		WEAPONTYPE_C4,
		WEAPONTYPE_PLACEHOLDER,
		WEAPONTYPE_GRENADE,
		WEAPONTYPE_UNKNOWN
	};

	struct RenderableInstance_t {
		uint8_t m_alpha;
		__forceinline RenderableInstance_t() : m_alpha{ 255ui8 } {}
	};

	class C_BaseEntity : public IClientEntity
	{
	public:
		NETVAR_OFFSET(GetIndex, "CBaseEntity", "m_bIsAutoaimTarget", +0x4, int);
		NETVAR(GetModelIndex, "CBaseEntity", "m_nModelIndex", unsigned);
		NETVAR(GetOwnerEntity, "CBaseEntity", "m_hOwnerEntity", CBaseHandle);

		void SetModelIndex(const int index)
		{
			get_vfunc<void(__thiscall*)(C_BaseEntity*, int)>(this, 75)(this, index);
		}

		void ValidateModelIndex()
		{
			get_vfunc<void(__thiscall*)(C_BaseEntity*)>(this, 54)(this);
		}

		bool IsPlayer(void) /* const */
		{
			return get_vfunc<bool(__thiscall *)(C_BaseEntity*)>(this, 157)(this);
		}
	};

	class C_BaseAnimating : public C_BaseEntity
	{
	public:
		void UpdateClientSideAnimation(void)
		{
			get_vfunc<void(__thiscall*)(C_BaseAnimating*)>(this, 223)(this);
		}

	};

	class C_BaseCombatCharacter : public C_BaseAnimating
	{
	public:
		NETVAR(GetWeapons, "CBaseCombatCharacter", "m_hMyWeapons", std::array<CBaseHandle, MAX_WEAPONS>);
		PNETVAR(GetWearables, "CBaseCombatCharacter", "m_hMyWearables", CBaseHandle);
	};

	class C_BaseCombatWeapon : public C_BaseAnimating
	{
	public:
		NETVAR(GetViewModelIndex, "CBaseCombatWeapon", "m_iViewModelIndex", int);
		NETVAR(GetWorldModelIndex, "CBaseCombatWeapon", "m_iWorldModelIndex", int);
		NETVAR(GetWorldDroppedModelIndex, "CBaseCombatWeapon", "m_iWorldDroppedModelIndex", int);
		NETVAR(GetWeaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel", CBaseHandle);
		NETVAR(GetClip1, "CBaseCombatWeapon", "m_iClip1", int);

		NETVAR(Index, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
		CCSWeapData* GetWeapInfo()
		{
			if (!this || this == NULL)
				return NULL;

			return get_vfunc<CCSWeapData* (__thiscall*)(C_BaseCombatWeapon*)>(this, 460)(this);
		}

		float GetInaccuracy() {
			return get_vfunc<float(__thiscall*)(void*)>(this, 482)(this);
		}

		float GetSpread() {
			return get_vfunc<float(__thiscall*)(void*)>(this, 452)(this);
		}

		void UpdateAccuracyPenalty() {
			get_vfunc<void(__thiscall*)(void*)>(this, 483)(this);
		}
	};

	class C_BaseAttributableItem : public C_BaseCombatWeapon
	{
	public:
		NETVAR(GetAccountID, "CBaseAttributableItem", "m_iAccountID", int);
		NETVAR(GetItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
		NETVAR(GetItemIDLow, "CBaseAttributableItem", "m_iItemIDLow", int);
		NETVAR(GetItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh", int);
		NETVAR(GetEntityQuality, "CBaseAttributableItem", "m_iEntityQuality", int);
		NETVAR(GetCustomName, "CBaseAttributableItem", "m_szCustomName", char[32]);
		NETVAR(GetFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned);
		NETVAR(GetFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned);
		NETVAR(GetFallbackWear, "CBaseAttributableItem", "m_flFallbackWear", float);
		NETVAR(GetFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned);
	};

#pragma pack(push, 1)
	class animstate_pose_param_cache_t
	{
	public:
		bool m_bInitialized; //0x0000
		char pad_01[3]; //0x0001
		int32_t m_iPoseParameter; //0x0004
		char* m_szPoseParameter; //0x0008
	}; //Size: 0x000C
#pragma pack(pop)

#pragma pack(push, 1)
	struct AimLayer
	{
		float m_flUnknown0;
		float m_flTotalTime;
		float m_flUnknown1;
		float m_flUnknown2;
		float m_flWeight;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct AimLayers
	{
		AimLayer layers[3];
	};
#pragma pack(pop)

	struct C_CSGOPlayerAnimState
	{
		void* vtable;
		bool m_bIsReset;
		bool m_bUnknownClientBool;
		char pad[2];
		int m_iSomeTickcount;
		float m_flFlashedStartTime;
		float m_flFlashedEndTime;
		AimLayers m_AimLayers;
		int m_nModelIndex;
		int m_iUnknownClientArr[3];
		C_BaseEntity* pBaseEntity;
		C_BaseCombatWeapon* pActiveWeapon;
		C_BaseCombatWeapon* pLastActiveWeapon;
		float m_flLastClientSideAnimationUpdateTime;
		int32_t m_iLastClientSideAnimationUpdateFramecount;
		float m_flLastClientSideAnimationUpdateTimeDelta;
		float m_flEyeYaw;
		float m_flPitch;
		float m_flGoalFeetYaw;
		float m_flCurrentFeetYaw;
		float m_flCurrentMoveDirGoalFeetDelta;
		float m_flGoalMoveDirGoalFeetDelta;
		float m_flFeetVelDirDelta;
		float pad_0094;
		float m_flFeetCycle;
		float m_flFeetWeight;
		float m_fUnknown2;
		float m_fDuckAmount;
		float m_flHitGroundCycle;
		float m_fUnknown3;
		Vector m_vOrigin;
		Vector m_vLastOrigin;
		Vector m_vVelocity;
		Vector m_vVelocityNormalized;
		Vector m_vecLastAcceleratingVelocity;
		float m_flSpeed;
		float m_flAbsVelocityZ;
		float m_flSpeedNormalized;
		float m_flRunningSpeed;
		float m_flDuckingSpeed;
		float m_flTimeSinceStartedMoving;
		float m_flTimeSinceStoppedMoving;
		bool m_bOnGround;
		bool m_bInHitGroundAnimation;
		char pad_010A[2];
		float m_flNextLowerBodyYawUpdateTime;
		float m_flTotalTimeInAir;
		float m_flStartJumpZOrigin;
		float m_flHitGroundWeight;
		float m_flGroundFraction;
		bool m_bJust_Landed;
		bool m_bJust_LeftGround;
		char pad_0120[2];
		float m_flDuckRate;
		bool m_bOnLadder;
		char pad_0128[3];
		float m_flLadderCycle;
		float m_flLadderWeight;
		bool m_bNotRunning;
		char pad_0135[3];
		bool m_bInBalanceAdjust;
		char pad_0141[3];
		CUtlVectorSimple m_Modifiers;
		int gap148[1];
		float m_flTimeOfLastInjury;
		float m_flLastSetupLeanCurtime;
		Vector m_vecLastSetupLeanVelocity;
		Vector m_vecSetupLeanVelocityDelta;
		Vector m_vecSetupLeanVelocityInterpolated;
		float m_flLeanWeight;
		int m_iUnknownIntArr2[2];
		bool m_bFlashed;
		char m_bFlashedPad[3];
		float m_flStrafeWeight;
		int m_iUnknownint3;
		float m_flStrafeCycle;
		int m_iStrafeSequence;
		bool m_bStrafing;
		char m_bStrafingPad[3];
		float m_flTotalStrafeTime;
		int m_iUnknownInt4;
		bool m_bUnknownBool__;
		bool m_bIsAccelerating;
		char pad_01AE[2];
		animstate_pose_param_cache_t m_arrPoseParameters[20];
		int m_iUnknownClientInt__;
		float m_flVelocityUnknown;
		int m_iMoveState;
		float m_flMovePlaybackRate;
		float m_flUnknownFL0;
		float m_flUnknownFL;
		float m_flUnknownFL1;
		float m_flMinYawServer;
		float m_flMaxYawServer;
		float m_flMaximumPitchServer;
		float m_flMinimumPitchServer;
		int m_iUnknownInt;
		char pad_02D0[84];
		float m_flEyePosZ;
		bool m_bIsDucked;
		char pad_0329[3];
		float m_flUnknownCap1;
		float m_flMinYaw;
		float m_flMaxYaw;
		float m_flMinPitch;
		float m_flMaxPitch;
		int m_iAnimsetVersion;

	};

	class C_BasePlayer : public C_BaseCombatCharacter
	{
	public:
		NETVAR(GetLifeState, "CBasePlayer", "m_lifeState", LifeState);
		NETVAR(GetViewModel, "CBasePlayer", "m_hViewModel[0]", CBaseHandle); 
		NETVAR(GetViewOffset, "CBasePlayer", "m_vecViewOffset[0]", Vector);
		NETVAR(GetOrigin, "CBaseEntity", "m_vecOrigin", Vector);
		NETVAR(GetTeamNumber, "CBaseEntity", "m_iTeamNum", int);	
		NETVAR(GetSimTime, "CBaseEntity", "m_flSimulationTime", float);
		NETVAR(GetAimPunchAngle, "CBasePlayer", "m_aimPunchAngle", QAngle);
		NETVAR(GetFlags, "CBasePlayer", "m_fFlags", EntityFlags);
		NETVAR(GetVelocity, "CBasePlayer", "m_vecVelocity[0]", Vector);
		NETVAR(GetLowerBodyYaw, "CCSPlayer", "m_flLowerBodyYawTarget", float_t);
		NETVAR(GetDuckAmount, "CCSPlayer", "m_flDuckAmount", float_t);
		NETVAR(GetEyeAngles, "CCSPlayer", "m_angEyeAngles[0]", QAngle);;
		NETVAR_OFFSET(GetOldSimTime, "CBaseEntity", "m_flSimulationTime", 0x4, float_t);

		NETVAR(GetWeaponActive, "CBaseCombatCharacter", "m_hActiveWeapon", CBaseHandle);
		NETVAR(HitboxSet, "CBaseAnimating", "m_nHitboxSet", int);
		int	GetMoveType() 
		{
			return *reinterpret_cast<int*> (reinterpret_cast<uintptr_t>(this) + 0x25C); //hazedumper
		}

		Vector BonePosition(int bone)
		{
			matrix3x4_t pMat[128];
			this->SetupBones(pMat, 128, 256, GetSimTime());
			return Vector(pMat[bone][0][3], pMat[bone][1][3], pMat[bone][2][3]);
		}

		Vector BonePositionFromMatrix(int bone, matrix3x4_t* pMat)
		{
			return Vector(pMat[bone][0][3], pMat[bone][1][3], pMat[bone][2][3]);
		}

		C_CSGOPlayerAnimState* GetPlayerAnimState()
		{
			return *reinterpret_cast<C_CSGOPlayerAnimState**>(this + 0x3914);
		}

		Vector GetEyePosition() { return this->GetOrigin() + this->GetViewOffset(); }
	};

	class C_BaseViewModel : public C_BaseAnimating
	{
	public:
		NETVAR(GetOwner, "CBaseViewModel", "m_hOwner", CBaseHandle);
		NETVAR(GetWeapon, "CBaseViewModel", "m_hWeapon", CBaseHandle);
		NETPROP(GetSequenceProp, "CBaseViewModel", "m_nSequence");
	};

	class C_PlayerResource
	{
	public:
		NETPROP(GetTeamProp, "CPlayerResource", "m_iTeam");
	};

	class C_CS_PlayerResource : public C_PlayerResource
	{
	public:
		NETVAR(GetRanks, "CCSPlayerResource", "m_iCompetitiveRanking", int[MAX_PLAYERS]);
		NETVAR(GetWins, "CCSPlayerResource", "m_iCompetitiveWins", int[MAX_PLAYERS]);
		NETVAR(GetClanTags, "CCSPlayerResource", "m_szClan", char[MAX_PLAYERS][32]);
		NETVAR(GetCoins, "CCSPlayerResource", "m_nActiveCoinRank", unsigned[MAX_PLAYERS]);
		NETVAR(GetMusicKits, "CCSPlayerResource", "m_nMusicID", unsigned[MAX_PLAYERS]);
	};

	class CBaseWeaponWorldModel : public C_BaseAnimating
	{
	};
}