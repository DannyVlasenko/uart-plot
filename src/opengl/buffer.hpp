#ifndef OPENGL_BUFFER_HPP
#define OPENGL_BUFFER_HPP

#include <span>
#include "glcall.hpp"

namespace opengl
{
	template <GLenum TargetE, typename T>
	class Buffer
	{
	public:
		static constexpr GLenum Target = TargetE;

		Buffer(std::span<const T> data, GLenum usage = GL_STATIC_DRAW) :
			mId(0),
			mCount(data.size())
		{
			GLCall(glGenBuffers(1, &mId));
			GLCall(glBindBuffer(Target, mId));
			GLCall(glBufferData(Target, data.size_bytes(), data.data(), usage));
		}

		Buffer(const Buffer& other) = delete;
		Buffer& operator=(const Buffer& other) = delete;

		Buffer(Buffer&& other) noexcept :
			mId(other.mId),
			mCount(other.mCount)
		{
			other.mId = 0;
			other.mCount = 0;
		}

		Buffer& operator=(Buffer&& other) noexcept
		{
			std::swap(mId, other.mId);
			std::swap(mCount, other.mCount);
			return *this;
		}

		~Buffer()
		{
			GLCall(glDeleteBuffers(1, &mId));
		}

		[[nodiscard]]
		auto count() const noexcept
		{
			return mCount;
		}

		void bind() const
		{
			GLCall(glBindBuffer(Target, mId));
		}

		static void unbind()
		{
			GLCall(glBindBuffer(Target, 0));
		}

	private:
		GLuint mId;
		typename std::span<T>::size_type mCount;
	};

	template <typename T>
	class VertexBuffer final : public Buffer<GL_ARRAY_BUFFER, T>
	{
	public:
		using Buffer<GL_ARRAY_BUFFER, T>::Buffer;
	};

	template <typename Container>
	VertexBuffer(Container data, GLenum usage = GL_STATIC_DRAW) -> VertexBuffer<std::remove_reference_t<decltype(data[0])>>;

	using IndexBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER, GLuint>;
}

#endif // OPENGL_BUFFER_HPP
