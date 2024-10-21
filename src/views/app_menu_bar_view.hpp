#pragma once

#include "view.hpp"

#include <string>
#include <span>

namespace views
{
	class IAppMenuBarViewModel
	{
	public:
		virtual ~IAppMenuBarViewModel() = default;
		virtual bool portConfigurationChecked() = 0;
		virtual void onPortConfigurationClicked() = 0;
		virtual bool bleConfigurationChecked() = 0;
		virtual void onBleConfigurationClicked() = 0;
		virtual void onNewSignalViewClicked() = 0;
		virtual void onNewSpectrumViewClicked() = 0;
	};

	class AppMenuBarView : public IView
	{
	public:
		explicit AppMenuBarView(IAppMenuBarViewModel& viewModel):
			mViewModel(viewModel)
		{}

		void render() const override;

	private:
		IAppMenuBarViewModel& mViewModel;
	};
}