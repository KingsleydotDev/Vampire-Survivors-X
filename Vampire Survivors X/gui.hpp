#pragma once
#include "includes.h"

namespace gui
{

    inline void RenderPlayerTab()
    {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "Local Player: %p", vars::pLocalPlayer);
        ImGui::Separator();

        // Safe toggle for God Mode
        ImGui::Checkbox("God Mode", &vars::bGodMode);

        // Informative note for the user
        if (!vars::pLocalPlayer)
            ImGui::TextWrapped("Waiting for player capture... (Move in-game)");
    }

    inline void RenderEconomyTab()
    {
        ImGui::Text("Economy Settings");
        ImGui::Separator();
        // Infinite Magnet Toggle
    }

    inline void RenderSettingsTab()
    {
        ImGui::Text("Cheat Settings");
        ImGui::Separator();

        if (ImGui::Button("Unload & Detach DLL", ImVec2(-1, 25))) {
            vars::bUnload = true;
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextDisabled("VampireInternal v1.1 - Minimal Stable Build");
        ImGui::TextDisabled("Menu Key: [INSERT]");
    }


    inline void Render()
    {
        // Toggle Menu
        if (GetAsyncKeyState(VK_INSERT) & 1) vars::bShowMenu = !vars::bShowMenu;
        if (!vars::bShowMenu) return;

        ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("VampireInternal", &vars::bShowMenu, ImGuiWindowFlags_NoCollapse))
        {
            if (ImGui::BeginTabBar("MainTabs"))
            {
                if (ImGui::BeginTabItem("Player")) {
                    RenderPlayerTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Economy")) {
                    RenderEconomyTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Settings")) {
                    RenderSettingsTab();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}