#include "app_menu_bar_view.hpp"
#include "imgui.h"

void views::AppMenuBarView::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Port"))
        {
            if (ImGui::MenuItem("Open", "CTRL+O")) {
                
            }
            ImGui::Separator();
            ImGui::MenuItem("COM11", nullptr, false);
            ImGui::MenuItem("COM12", nullptr, false);
            ImGui::MenuItem("COM13", nullptr, false);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("New Signal Plot", "CTRL+P")) {}
            if (ImGui::MenuItem("New Spectrum Plot", "CTRL+U")) {}
            ImGui::Separator();
            ImGui::MenuItem("Signal 1", nullptr, false);
            ImGui::MenuItem("Signal 2", nullptr, false);
            ImGui::MenuItem("Spectrum 1", nullptr, false);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
