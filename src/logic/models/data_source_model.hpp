#pragma once
#include <memory>
#include <string_view>
#include <unordered_map>
#include <ranges>

namespace logic
{
	class IDataSource
	{
	public:
		virtual ~IDataSource() = default;
		virtual void update() = 0;
		virtual [[nodiscard]] const std::string_view name() const noexcept = 0;
		virtual [[nodiscard]] size_t size() const noexcept = 0;
		virtual [[nodiscard]] std::span<const double> getData(size_t offset = 0, size_t length = std::numeric_limits<size_t>::max()) const noexcept = 0;
	};

	class DataSourceModel
	{
	public:
		void addDataSource(std::unique_ptr<IDataSource> dataSource)
		{
			const auto name = dataSource->name();
			mDataSources.insert_or_assign(name.data(), std::move(dataSource));
		}

		void removeDataSource(const std::string& name)
		{
			mDataSources.erase(name);
		}

		void update()
		{
			for (auto& dataSource : mDataSources | std::views::values) {
				dataSource->update();
			}
		}

		[[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<IDataSource>>& dataSources() const
		{
			return mDataSources;
		}

	private:
		std::unordered_map<std::string, std::unique_ptr<IDataSource>> mDataSources;
	};
}