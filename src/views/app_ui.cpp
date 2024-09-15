#include "app_ui.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "implot.h"

views::AppUI::AppUI(const glfw::Window& window, const IMainViewModel& viewModel):
	mViewModel(viewModel)
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
	ImPlot::DestroyContext();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void views::AppUI::render() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	for (const auto& view : mViewModel)
	{
		view->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
