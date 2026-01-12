#pragma once
#include "includes.h"

namespace gui
{
    inline void RenderWatermark()
    {
        if (!vars::bWatermark) return;

        time_t rawtime;
        struct tm timeinfo;
        char timeBuffer[80];
        time(&rawtime);
        localtime_s(&timeinfo, &rawtime);
        strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeinfo);

        std::string redText = "[DEV BUILD]";
        char whiteBuffer[128];
        sprintf_s(whiteBuffer, sizeof(whiteBuffer), " | %d FPS | %s | Vampire Survivors X | By Kingsley", (int)ImGui::GetIO().Framerate, timeBuffer);
        std::string whiteText = whiteBuffer;

        ImVec2 redSize = ImGui::CalcTextSize(redText.c_str());
        ImVec2 whiteSize = ImGui::CalcTextSize(whiteText.c_str());

        float totalWidth = redSize.x + whiteSize.x;
        float padding = 5.0f;
        float x = ImGui::GetIO().DisplaySize.x - totalWidth - 15.0f; // 15 px from right edge
        float y = 15.0f;

        // Draw Background
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x - padding, y - padding), ImVec2(x + totalWidth + padding, y + redSize.y + padding), ImColor(0, 0, 0, 200), 4.0f);

        // Draw Red Text (with shadow)
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1, y + 1), ImColor(0, 0, 0), redText.c_str());
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), ImColor(255, 0, 0), redText.c_str());

        // Draw White Text (with shadow)
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + redSize.x + 1, y + 1), ImColor(0, 0, 0), whiteText.c_str());
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + redSize.x, y), ImColor(255, 255, 255), whiteText.c_str());
	}

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
		ImGui::Checkbox("Show Watermark", &vars::bWatermark);
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
		RenderWatermark();
        // Toggle Menu
        if (GetAsyncKeyState(VK_INSERT) & 1) vars::bShowMenu = !vars::bShowMenu;
        if (!vars::bShowMenu) return;

        ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Vampire Survivors X Internal", &vars::bShowMenu, ImGuiWindowFlags_NoCollapse))
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