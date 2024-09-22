#include "app_menu_bar_view.hpp"
#include "imgui.h"

void views::AppMenuBarView::render() const
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Port Configuration Window", nullptr, mViewModel.portConfigurationChecked())) {
                mViewModel.onPortConfigurationClicked();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Add Signal Plot")) {
                mViewModel.onNewSignalViewClicked();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
