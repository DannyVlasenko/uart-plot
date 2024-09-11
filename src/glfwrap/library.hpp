#ifndef GLFW_LIBRARY_HPP
#define GLFW_LIBRARY_HPP

namespace glfw
{
	class Library
	{
	public:
		Library();

		Library(const Library& other) = delete;
		Library(Library&& other) noexcept = delete;
		Library& operator=(const Library& other) = delete;
		Library& operator=(Library&& other) noexcept = delete;

		~Library();
	};

	class EnableLibrary
	{
		[[maybe_unused]]
		[[no_unique_address]]
		Library mGlfwLib{};
	};
}
#endif // GLFW_LIBRARY_HPP
