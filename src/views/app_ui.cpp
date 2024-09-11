#include "app_ui.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "implot.h"

views::AppUI::AppUI(const glfw::Window& window)
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

void views::AppUI::add_view(std::unique_ptr<IView> view)
{
	mViews.push_back(std::move(view));
}

void views::AppUI::render() const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	for (const auto& view : mViews)
	{
		view->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
