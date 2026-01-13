#include "includes.h"
#include "hooks.hpp"
#include "gui.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (vars::bShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(vars::oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&vars::pDevice)))
        {
            vars::pDevice->GetImmediateContext(&vars::pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            vars::window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            vars::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &vars::mainRenderTargetView);
            pBackBuffer->Release();
            vars::oWndProc = (WNDPROC)SetWindowLongPtr(vars::window, GWLP_WNDPROC, (LONG_PTR)WndProc);

            ImGui::CreateContext();
            ImGui_ImplWin32_Init(vars::window);
            ImGui_ImplDX11_Init(vars::pDevice, vars::pContext);
            init = true;
        }
        return vars::oPresent(pSwapChain, SyncInterval, Flags);
    }

    // IL2CPP Safety: Attach thread before rendering/calling methods
    if (hooks::_il2cpp_domain_get && hooks::_il2cpp_thread_attach) {
        void* domain = hooks::_il2cpp_domain_get();
        if (domain) {
            hooks::_il2cpp_thread_attach(domain);
        }
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    gui::Render();

    ImGui::Render();
    vars::pContext->OMSetRenderTargets(1, &vars::mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return vars::oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    printf("[VampireInternal] Console Allocated. Version: 1.0\n");

    printf("[VampireInternal] Waiting for GameAssembly.dll...\n");
    while (GetModuleHandleA("GameAssembly.dll") == nullptr)
    {
        Sleep(100);
    }
    printf("[VampireInternal] GameAssembly.dll found at: %p\n", GetModuleHandleA("GameAssembly.dll"));

    printf("[VampireInternal] Initializing Kiero...\n");
    if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
    {
        kiero::bind(8, (void**)&vars::oPresent, hkPresent);

        hooks::Init();
        printf("[VampireInternal] Initialization Complete. Press INSERT for menu.\n");
    }
    else
    {
        printf("[VampireInternal] CRITICAL ERROR: Kiero initialization failed!\n");
    }

    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    }
    return TRUE;
}