#pragma once
#include "includes.h"

namespace hooks
{
    // il2cpp threading exports
    typedef void* (*t_il2cpp_domain_get)();
    inline t_il2cpp_domain_get _il2cpp_domain_get = nullptr;
    typedef void* (*t_il2cpp_thread_attach)(void* domain);
    inline t_il2cpp_thread_attach _il2cpp_thread_attach = nullptr;


    // function pointers


    // hooks

    // captures the player instance every frame
    typedef void(__fastcall* tOnUpdate)(void* __this, void* method);
    inline tOnUpdate oOnUpdate = nullptr;

    inline void __fastcall hkOnUpdate(void* __this, void* method)
    {
        if (__this) {
            vars::pLocalPlayer = __this;
        }
        return oOnUpdate(__this, method);
    }

    // blocks incoming damage if god mode is toggled
    typedef void(__fastcall* tOnGetDamaged)(void* __this, void* damageSignal, void* method);
    inline tOnGetDamaged oOnGetDamaged = nullptr;

    inline void __fastcall hkOnGetDamaged(void* __this, void* damageSignal, void* method)
    {
        if (vars::bGodMode) return;
        return oOnGetDamaged(__this, damageSignal, method);
    }

    inline void Init()
    {
        offsets::GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
        if (!offsets::GameAssembly) return;

        MH_Initialize();

#define CREATE_HOOK(addr, hook, orig, name) { \
            uintptr_t targetAddr = offsets::GameAssembly + addr; \
            if (MH_CreateHook((void*)targetAddr, &hook, (LPVOID*)&orig) == MH_OK) { \
                MH_EnableHook((void*)targetAddr); \
                printf("[DEBUG] Hooked %s\n", name); \
            } \
        }

        // using the rvas verified from your ida dump
        CREATE_HOOK(offsets::player::OnUpdate, hkOnUpdate, oOnUpdate, "OnUpdate");
        CREATE_HOOK(offsets::player::OnGetDamaged, hkOnGetDamaged, oOnGetDamaged, "OnGetDamaged");

#undef CREATE_HOOK
    }
}