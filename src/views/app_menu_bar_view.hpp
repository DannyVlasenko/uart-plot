#pragma once

#include "view.hpp"

namespace views
{
	class AppMenuBarView : public IView
	{
	public:
		void render() override;
	};
}