#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>


#include <iostream> // Added for console
#include <cstdio>   // Added for freopen

// Libraries
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "kiero/minhook/include/MinHook.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

// Project Files
#include "offsets.hpp"
#include "vars.hpp"