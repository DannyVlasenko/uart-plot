#include "views/signal_view.hpp"

#include "imgui.h"
#include "implot.h"

namespace views
{
	void SignalView::render() const
	{
		ImGui::Begin("Signal");
		ImPlot::BeginPlot("Samples");
		ImPlot::EndPlot();
		ImGui::End();
	}
}
