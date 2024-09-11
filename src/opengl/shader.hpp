#ifndef OPENGL_SHADER_HPP
#define OPENGL_SHADER_HPP

#include "glcall.hpp"
#include <iterator>
#include <ranges>

#include "glm.hpp"

namespace opengl
{
	struct ShaderSource
	{
		GLuint type;
		const char* code;
	};

	class Shader
	{
	public:
		explicit Shader(const ShaderSource& shader_source);
		Shader(GLuint type, const char* source);
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		~Shader();

		operator GLuint() const noexcept
		{
			return mId;
		}

	private:
		GLuint mId{ 0 };
	};

#ifdef __cpp_lib_ranges
	constexpr auto shader_view = std::ranges::views::transform([](const auto& source) { return Shader(source); });
#endif

	class Program
	{
	public:
		template <std::input_iterator InIt>
		Program(InIt begin, InIt end) :
			mId(create_program())
		{
			while (begin != end) {
				attach(*begin++);
			}
			link_validate();
		}

#ifdef __cpp_lib_ranges
		template <std::ranges::range ShRng>
		Program(ShRng shaders) :
			mId(create_program())
		{
			for (const auto& shader : shaders) {
				attach(shader);
			}
			link_validate();
		}
#endif

		template <typename... Sh> requires (std::is_same_v<Shader, Sh> && ...)
			Program(const Sh&... shader) :
			mId(create_program())
		{
			(attach(shader), ...);
			link_validate();
		}

		Program(const Program& other) = delete;
		Program& operator=(const Program& other) = delete;
		Program(Program&& other) noexcept;
		Program& operator=(Program&& other) noexcept;
		~Program();

		void bind() const;

		static void unbind();

		void set_uniform(const char* name, float f) const;
		void set_uniform(const char* name, const glm::vec3& vec) const;
		void set_uniform(const char* name, const glm::mat4& mat) const;
		void set_uniform(const char* name, const glm::mat3& mat) const;

	private:
		GLuint mId;

		static GLuint create_program();
		void attach(const Shader& shader);
		void link_validate() const;
	};
}
#endif // OPENGL_SHADER_HPP
