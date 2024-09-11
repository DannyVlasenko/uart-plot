#ifndef OPENGL_MESH_HPP
#define OPENGL_MESH_HPP

#include <filesystem>
#include <vec3.hpp>
#include <vector>

#include "buffer.hpp"
#include "vertex_array.hpp"

namespace opengl
{
	struct VertexNormal
	{
		glm::vec3 position;
		glm::vec3 normal;

		static VertexBufferLayout layout();
	};

	struct VertexSimple
	{
		glm::vec3 position;

		static VertexBufferLayout layout();
	};

	template <typename Vertex>
	class Mesh
	{
	public:
		Mesh(std::span<Vertex> vertices, std::span<GLuint> indices) :
			mVertexBuffer(vertices),
			mIndexBuffer(indices)
		{
			mVertexArray.add_buffer(mVertexBuffer, Vertex::layout());
		}

		void draw(GLenum mode = GL_TRIANGLES) const
		{
			mVertexArray.bind();
			mIndexBuffer.bind();
			GLCall(glDrawElements(mode, mIndexBuffer.count(), GL_UNSIGNED_INT, nullptr));
			mIndexBuffer.unbind();
			mVertexArray.unbind();
		}

	private:
		VertexArray mVertexArray;
		VertexBuffer<Vertex> mVertexBuffer;
		IndexBuffer mIndexBuffer;
	};

	Mesh<VertexNormal> cube_mesh();

	std::vector<Mesh<VertexNormal>> load_from_file(const std::filesystem::path& file);
}
#endif // OPENGL_MESH_HPP
