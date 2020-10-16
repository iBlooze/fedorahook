#pragma once

namespace sdk
{
    class IPanel
    {
    public:
        const char* GetName(unsigned int vguiPanel)
        {
            typedef const char* (__thiscall* fn)(void*, unsigned int);
            return get_vfunc<fn>(this, 36)(this, vguiPanel);
        }
#ifdef GetClassName
#undef GetClassName
#endif
        const char* GetClassName(unsigned int vguiPanel)
        {
            typedef const char* (__thiscall* fn)(void*, unsigned int);
            return get_vfunc<fn>(this, 37)(this, vguiPanel);
        }
    };

}