#include "app_ui.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "implot.h"
#include "glfwrap/window.hpp"

views::AppUI::AppUI(const glfw::Window& window, IAppMenuBarViewModel& menuBarModel, const IDockAreaViewModel& dockAreaModel):
	mMenuBarView(menuBarModel),
	mDockViewModel(dockAreaModel)
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImPlot::CreateContext();
	ImGui::GetStyle().ScaleAllSizes(1.5);
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\CascadiaMono.ttf", 16.0f * 1.5f);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

views::AppUI::~AppUI()
{
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void views::AppUI::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	mMenuBarView.render();
	for (const auto * view : mDockViewModel.views())
	{
		view->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
