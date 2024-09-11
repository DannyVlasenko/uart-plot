#ifndef OPENGL_VERTEX_ARRAY_HPP
#define OPENGL_VERTEX_ARRAY_HPP

#include <vector>

namespace opengl
{
	template <typename T>
	constexpr GLenum gl_type() = delete;

	template <>
	constexpr GLenum gl_type<float>()
	{
		return GL_FLOAT;
	}

	template <>
	constexpr GLenum gl_type<unsigned int>()
	{
		return GL_UNSIGNED_INT;
	}

	template <>
	constexpr GLenum gl_type<unsigned char>()
	{
		return GL_UNSIGNED_BYTE;
	}

	class VertexBufferLayout
	{
	public:
		struct Element
		{
			Element(GLuint count, GLenum type, GLboolean normalized, GLuint offset) :
				count(count),
				type(type),
				normalized(normalized),
				offset(offset)
			{}

			GLuint count;
			GLenum type;
			GLboolean normalized;
			GLuint offset;
		};

		template <typename T>
		VertexBufferLayout& push(GLuint count, GLboolean normalized = GL_FALSE)
		{
			mElements.emplace_back(count, gl_type<T>(), normalized, mStride);
			mStride += sizeof(T) * count;
			return *this;
		}

		[[nodiscard]]
		GLuint stride() const noexcept
		{
			return mStride;
		}

		[[nodiscard]]
		const std::vector<Element>& elements() const noexcept
		{
			return mElements;
		}

	private:
		std::vector<Element> mElements;
		GLuint mStride{ 0 };
	};

	class VertexArray
	{
	public:
		VertexArray()
		{
			GLCall(glGenVertexArrays(1, &mId));
		}

		VertexArray(const VertexArray& other) = delete;
		VertexArray& operator=(const VertexArray& other) = delete;

		VertexArray(VertexArray&& other) noexcept :
			mId(other.mId)
		{
			other.mId = 0;
		}

		VertexArray& operator=(VertexArray&& other) noexcept
		{
			std::swap(mId, other.mId);
			return *this;
		}

		~VertexArray()
		{
			GLCall(glDeleteVertexArrays(1, &mId));
		}

		void bind() const
		{
			GLCall(glBindVertexArray(mId));
		}

		static void unbind()
		{
			GLCall(glBindVertexArray(0));
		}

		VertexArray& add_buffer(const auto& vb, const VertexBufferLayout& layout)
		{
			bind();
			vb.bind();
			for (GLuint i = 0; i < layout.elements().size(); ++i)
			{
				GLCall(glEnableVertexAttribArray(i));
				GLCall(glVertexAttribPointer(i, layout.elements()[i].count, layout.elements()[i].type,
					layout.elements()[i].normalized, layout.stride(),
					(const void*)(layout.elements()[i].offset)));
			}
			return *this;
		}

	private:
		GLuint mId{ 0 };
	};
}

#endif // OPENGL_VERTEX_ARRAY_HPP
