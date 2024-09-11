#include "mesh.hpp"

namespace opengl
{
	VertexBufferLayout VertexNormal::layout()
	{
		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(3);
		return layout;
	}

	VertexBufferLayout VertexSimple::layout()
	{
		VertexBufferLayout layout;
		layout.push<float>(3);
		return layout;
	}

	Mesh<VertexNormal> cube_mesh()
	{
		VertexNormal vertices[] = {
			//0
			{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			 {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			 {{0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

			//4
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
			 {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
			 {{0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},

			//8
			{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},

			//12
			 {{0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}},
			 {{0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}},
			 {{0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}},
			 {{0.5f,  0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}},

			 //16
			 {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
			  {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
			  {{0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}},
			 {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}},

			 //20
			 {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			  {{0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
			 {{-0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}}
		};

		GLuint indices[] =
		{
			 0,  1,  2,  0,  2,  3,
			 4,  7,  6,  4,  6,  5,
			 8,  9, 10,  8, 10, 11,
			12, 15, 14, 12, 14, 13,
			16, 17, 18, 16, 18, 19,
			20, 23, 22, 20, 22, 21
		};

		return { vertices, indices };
	}
}
