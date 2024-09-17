#ifndef VIEWS_GLOBAL_LIGHT_VIEW_HPP
#define VIEWS_GLOBAL_LIGHT_VIEW_HPP

#include <span>
#include <string>

#include "view.hpp"

namespace views
{
	class IPortConfigurationViewModel
	{
	public:
		virtual ~IPortConfigurationViewModel() = default;
		virtual void onPortRefreshButtonClicked() = 0;
		virtual std::span<const std::string> availablePorts() const noexcept = 0;
		virtual size_t& selectedAvailablePort() noexcept = 0;
		virtual bool portOpenEnabled() noexcept = 0;
		virtual void onPortOpenButtonClicked() = 0;
		virtual std::span<const std::string> openedPorts() const noexcept = 0;
		virtual size_t& selectedOpenedPort() noexcept = 0;
		virtual bool portControlsEnabled() noexcept = 0;
		virtual std::span<const std::string> baudRatesList() const noexcept = 0;
		virtual size_t& selectedBaudRate() noexcept = 0;
		virtual std::span<const std::string> dataBitsList() const noexcept = 0;
		virtual size_t& selectedDataBits() noexcept = 0;
		virtual std::span<const std::string> paritiesList() const noexcept = 0;
		virtual size_t& selectedParity() noexcept = 0;
		virtual std::span<const std::string> stopBitsList() const noexcept = 0;
		virtual size_t& selectedStopBits() noexcept = 0;
		virtual std::span<const std::string> flowControlsList() const noexcept = 0;
		virtual size_t& selectedFlowControl() noexcept = 0;
		virtual void onPortCloseButtonClicked() = 0;
	};

	class PortConfigurationView final : public IView
	{
	public:
		PortConfigurationView(IPortConfigurationViewModel& view_model) :
			mViewModel(view_model)
		{}

		void render() const override;

	private:
		IPortConfigurationViewModel& mViewModel;
	};
}
#endif // VIEWS_GLOBAL_LIGHT_VIEW_HPP
