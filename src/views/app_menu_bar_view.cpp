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
            if (ImGui::BeginMenu("Bluetooth Devices")) {
                if (ImGui::MenuItem("Device Selection Window", nullptr, mViewModel.bleConfigurationChecked())) {
                    mViewModel.onBleConfigurationClicked();
                }
                ImGui::Separator();

                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Add Signal Plot")) {
                mViewModel.onNewSignalViewClicked();
            }
            if (ImGui::MenuItem("Add Spectrum Plot")) {
                mViewModel.onNewSpectrumViewClicked();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
