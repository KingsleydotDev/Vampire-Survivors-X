#pragma once
#include <Windows.h>
#include <cstdint>

namespace offsets
{
    inline uintptr_t GameAssembly = 0;

    namespace player
    {
        // VampireSurvivors.Objects.Characters.CharacterController$$OnUpdate
        inline uintptr_t OnUpdate = 0x75C48E0;

        // VampireSurvivors.Objects.Characters.CharacterController$$OnGetDamaged
        inline uintptr_t OnGetDamaged = 0x75D96E0;
    }
}