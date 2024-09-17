#include "app_menu_bar_view.hpp"
#include "imgui.h"

void views::AppMenuBarView::render() const
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Port Configuration Window", "CTRL+P", mViewModel.portConfigurationChecked())) {
                mViewModel.onPortConfigurationClicked();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Add Signal Plot", "CTRL+I")) {
                mViewModel.onNewSignalViewClicked();
            }
            if (ImGui::MenuItem("Add Spectrum Plot", "CTRL+O")) {
                mViewModel.onNewSpectrumViewClicked();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
