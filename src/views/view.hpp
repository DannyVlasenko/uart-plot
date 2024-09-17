#ifndef VIEWS_VIEW_HPP
#define VIEWS_VIEW_HPP

namespace views
{
	class IView
	{
	public:
		virtual ~IView() = default;
		virtual void render() const = 0;
	};
}

#endif // VIEWS_VIEW_HPP
