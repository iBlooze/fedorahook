#pragma once
#pragma once

typedef float Quaternion[4];
typedef float RadianEuler[3];

#define MAX_QPATH  260

#define BONE_CALCULATE_MASK             0x1F
#define BONE_PHYSICALLY_SIMULATED       0x01    // bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL         0x02    // procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL          0x04    // bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE        0x08    // bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER      0x10    // bone aligns to the screen, constrained by it's own axis.

#define BONE_USED_MASK                  0x0007FF00
#define BONE_USED_BY_ANYTHING           0x0007FF00
#define BONE_USED_BY_HITBOX             0x00000100    // bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT         0x00000200    // bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK        0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1        0x00000800    
#define BONE_USED_BY_VERTEX_LOD2        0x00001000  
#define BONE_USED_BY_VERTEX_LOD3        0x00002000
#define BONE_USED_BY_VERTEX_LOD4        0x00004000
#define BONE_USED_BY_VERTEX_LOD5        0x00008000
#define BONE_USED_BY_VERTEX_LOD6        0x00010000
#define BONE_USED_BY_VERTEX_LOD7        0x00020000
#define BONE_USED_BY_BONE_MERGE         0x00040000    // bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8
#define MAXSTUDIOBONES		128		// total bones actually used

#define BONE_TYPE_MASK                  0x00F00000
#define BONE_FIXED_ALIGNMENT            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS          0x00200000    // Vector48
#define BONE_HAS_SAVEFRAME_ROT64        0x00400000    // Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32        0x00800000    // Quaternion32


#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4    
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR 10

enum modtype_t
{
    mod_bad = 0,
    mod_brush,
    mod_sprite,
    mod_studio
};

enum Hitboxes
{
    HITBOX_HEAD,
    HITBOX_NECK,
    HITBOX_PELVIS,
    HITBOX_STOMACH,
    HITBOX_LOWER_CHEST,
    HITBOX_CHEST,
    HITBOX_UPPER_CHEST,
    HITBOX_RIGHT_THIGH,
    HITBOX_LEFT_THIGH,
    HITBOX_RIGHT_CALF,
    HITBOX_LEFT_CALF,
    HITBOX_RIGHT_FOOT,
    HITBOX_LEFT_FOOT,
    HITBOX_RIGHT_HAND,
    HITBOX_LEFT_HAND,
    HITBOX_RIGHT_UPPER_ARM,
    HITBOX_RIGHT_FOREARM,
    HITBOX_LEFT_UPPER_ARM,
    HITBOX_LEFT_FOREARM,
    HITBOX_MAX
};


typedef unsigned short MDLHandle_t;


struct mstudiobone_t
{
    int                    sznameindex;
    inline char* const    pszName(void) const { return ((char*)this) + sznameindex; }
    int                    parent;
    int                    bonecontroller[6];    // bone controller index, -1 == none
    Vector                 pos;
    Quaternion             quat;
    RadianEuler            rot;
    // compression scale
    Vector                 posscale;
    Vector                 rotscale;

    matrix3x4_t            poseToBone;
    Quaternion             qAlignment;
    int                    flags;
    int                    proctype;
    int                    procindex;
    mutable int            physicsbone;
    inline void* pProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((byte*)this) + procindex); };
    int                    surfacepropidx;
    inline char* const    pszSurfaceProp(void) const { return ((char*)this) + surfacepropidx; }
    inline int             GetSurfaceProp(void) const { return surfacepropLookup; }

    int                    contents;
    int                    surfacepropLookup;
    int                    m_iPad01[7];

    mstudiobone_t() {}
private:
    // No copy constructors allowed
    mstudiobone_t(const mstudiobone_t& vOther);
};


struct mstudiobbox_t
{
    int bone;
    int group;
    Vector mins;
    Vector maxs;
    int name_index;
    int pad01[3];
    float radius;
    int pad02[4];
};

struct mstudiohitboxset_t
{
    int name_index;
    int hitbox_count;
    int hitbox_index;

    inline char* const GetName(void) const {
        return ((char*)this) + name_index;
    }
    inline mstudiobbox_t* GetHitbox(int i) const {
        return (mstudiobbox_t*)(((unsigned char*)this) + hitbox_index) + i;
    }
};

struct model_t
{
    void* fnHandle;               //0x0000 
    char    szName[260];            //0x0004 
    __int32 nLoadFlags;             //0x0108 
    __int32 nServerCount;           //0x010C 
    __int32 type;                   //0x0110 
    __int32 flags;                  //0x0114 
    Vector  vecMins;                //0x0118 
    Vector  vecMaxs;                //0x0124 
    float   radius;                 //0x0130 
    char    pad[0x1C];              //0x0134
};//Size=0x0150

class studiohdr_t
{
public:
    int id;
    int version;
    long checksum;
    char name_char_array[64];
    int length;
    Vector eye_pos;
    Vector illium_pos;
    Vector hull_mins;
    Vector hull_maxs;
    Vector mins;
    Vector maxs;
    int flags;
    int bones_count;
    int bone_index;
    int bone_controllers_count;
    int bone_controller_index;
    int hitbox_sets_count;
    int hitbox_set_index;
    int local_anim_count;
    int local_anim_index;
    int local_seq_count;
    int local_seq_index;
    int activity_list_version;
    int events_indexed;
    int textures_count;
    int texture_index;

    mstudiohitboxset_t* GetHitboxSet(int i) {
        if (i > hitbox_sets_count) return nullptr;
        return (mstudiohitboxset_t*)((uint8_t*)this + hitbox_set_index) + i;
    }
    mstudiobone_t* GetBone(int i) {
        if (i > bones_count) return nullptr;
        return (mstudiobone_t*)((uint8_t*)this + bone_index) + i;
    }
};