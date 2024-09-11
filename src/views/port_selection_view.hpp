#ifndef VIEWS_GLOBAL_LIGHT_VIEW_HPP
#define VIEWS_GLOBAL_LIGHT_VIEW_HPP

#include <span>
#include <string_view>

#include "view.hpp"

namespace views
{
	class IPortSelectionViewModel
	{
	public:
		virtual ~IPortSelectionViewModel() = default;

		virtual const std::span<const std::string_view>& portsDescriptions() const noexcept = 0;
		virtual size_t& selectedPort() noexcept = 0;
		virtual bool portControlsEnabled() noexcept = 0;
		virtual const std::span<const std::string_view>& baudRatesList() const noexcept = 0;
		virtual size_t& selectedBaudRate() noexcept = 0;
		virtual const std::span<const std::string_view>& dataBitsList() const noexcept = 0;
		virtual size_t& selectedDataBits() noexcept = 0;
		virtual const std::span<const std::string_view>& paritiesList() const noexcept = 0;
		virtual size_t& selectedParity() noexcept = 0;
		virtual const std::span<const std::string_view>& stopBitsList() const noexcept = 0;
		virtual size_t& selectedStopBits() noexcept = 0;
		virtual const std::span<const std::string_view>& flowControlsList() const noexcept = 0;
		virtual size_t& selectedFlowControl() noexcept = 0;
		virtual const char* portOperationButtonText() noexcept = 0;
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
