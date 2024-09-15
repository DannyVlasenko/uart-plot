#ifndef VIEWS_GLOBAL_LIGHT_VIEW_HPP
#define VIEWS_GLOBAL_LIGHT_VIEW_HPP

#include <span>
#include <string>

#include "view.hpp"

namespace views
{
	class IPortSelectionViewModel
	{
	public:
		virtual ~IPortSelectionViewModel() = default;

		virtual std::span<const std::string> portsDescriptions() const noexcept = 0;
		virtual size_t& selectedPort() noexcept = 0;
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
		virtual const char* portOperationButtonText() const noexcept = 0;
		virtual void onPortOperationButtonClicked() noexcept = 0;
	};

	class PortSelectionView final : public IView
	{
	public:
		PortSelectionView(IPortSelectionViewModel& view_model) :
			mViewModel(view_model)
		{}

		void render() override;

	private:
		IPortSelectionViewModel& mViewModel;
	};
}
#endif // VIEWS_GLOBAL_LIGHT_VIEW_HPP
