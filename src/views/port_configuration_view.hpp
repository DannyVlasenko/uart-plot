#ifndef VIEWS_GLOBAL_LIGHT_VIEW_HPP
#define VIEWS_GLOBAL_LIGHT_VIEW_HPP

#include <span>
#include <string>
#include <vec4.hpp>

#include "view.hpp"

namespace views
{
	struct PortLogEntry
	{
		glm::vec4 Color;
		std::string Text;
	};

	class IPortConfigurationViewModel
	{
	public:
		virtual ~IPortConfigurationViewModel() = default;
		virtual void onPortRefreshButtonClicked() = 0;
		virtual [[nodiscard]] std::span<const std::string> availablePorts() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedAvailablePort() noexcept = 0;
		virtual [[nodiscard]] bool portOpenEnabled() noexcept = 0;
		virtual void onPortOpenButtonClicked() = 0;
		virtual [[nodiscard]] std::span<const std::string> openedPorts() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedOpenedPort() noexcept = 0;
		virtual [[nodiscard]] bool portControlsEnabled() noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> baudRatesList() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedBaudRate() noexcept = 0;
		virtual [[nodiscard]] int& dataBits() noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> paritiesList() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedParity() noexcept = 0;
		virtual [[nodiscard]] std::span<const std::string> stopBitsList() const noexcept = 0;
		virtual [[nodiscard]] size_t& selectedStopBits() noexcept = 0;
		virtual void onPortCloseButtonClicked() = 0;
		virtual void onClearLogClicked() = 0;
		virtual [[nodiscard]] std::span<const PortLogEntry> portLogEntries() = 0;
		virtual void onReadParamsClicked() = 0;
		virtual void onWriteParamsClicked() = 0;
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
