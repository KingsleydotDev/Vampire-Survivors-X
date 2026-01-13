#pragma once
#include <Windows.h>
#include <d3d11.h>

// Typedef for the Present function hook
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

namespace vars
{
    // render globals
    inline HWND window = NULL;
    inline WNDPROC oWndProc = nullptr;
    inline Present oPresent = nullptr;
    inline ID3D11Device* pDevice = NULL;
    inline ID3D11DeviceContext* pContext = NULL;
    inline ID3D11RenderTargetView* mainRenderTargetView = NULL;

    // menu states
    inline bool bShowMenu = true;
    inline bool bWatermark = true;
    inline bool bUnload = false; // add this at some point if i cba
    inline void* pLocalPlayer = nullptr; // Captured via OnUpdate

    // toggles
    inline bool bGodMode = false;
    inline bool bInfiniteMagnet = false;



}