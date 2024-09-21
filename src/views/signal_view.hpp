#pragma once

#include "view.hpp"

namespace views
{
	class SignalView : public IView
	{
	public:
		void render() const override;
	};
}
