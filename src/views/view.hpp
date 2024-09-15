#ifndef VIEWS_VIEW_HPP
#define VIEWS_VIEW_HPP

namespace views
{
	class IView
	{
	public:
		virtual ~IView() = default;
		virtual void render() = 0;
	};

	class IViewIterator
	{
	public:
		explicit IViewIterator(IView* view):
			mView(view)
		{}
		IViewIterator& operator++() {

		}
		const IView& operator*() const;
		friend void swap(IViewIterator& lhs, IViewIterator& rhs) noexcept {
			std::swap(lhs.mView, rhs.mView);
		}
		friend bool operator==(const IViewIterator&, const IViewIterator&);
		friend bool operator!=(const IViewIterator&, const IViewIterator&);
	private:
		IView* mView;
	};
}

#endif // VIEWS_VIEW_HPP
