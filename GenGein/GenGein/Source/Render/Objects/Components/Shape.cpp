#include "gl_core_4_4.h"

#include <vector>
#include <string>
#include <glm\glm.hpp>

#include "Input\Console\Console.h"
#include "Shape.h"

using C_LOG_TYPE = Console::LOG_TYPE;

using namespace glm;

Shape::Shape()
{}

Shape::~Shape()
{
	glDeleteVertexArrays(1, &m_bufferInfo.m_VAO);
	glDeleteBuffers(1, &m_bufferInfo.m_VBO);
}

void Shape::Create(const ShapeType a_type)
{
	m_shapeType = a_type;

	std::vector<VertexProperty> vertices;
	std::vector<unsigned int>	indices;

	// #SECRET: Beefy boi returns a big chunk of dates
	CreateShapeProperties(vertices, indices);

	m_bufferInfo.m_IndexCount = indices.size();

	unsigned int& vao = m_bufferInfo.m_VAO;
	unsigned int& vbo = m_bufferInfo.m_VBO;
	unsigned int& ibo = m_bufferInfo.m_IBO;

	size_t vertMemSize = vertices.size() * sizeof(VertexProperty);
	size_t indexMemSize = indices.size() * sizeof(unsigned int);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertMemSize, &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexMemSize, &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	void* positMemOffset = (void*)offsetof(VertexProperty, position);
	void* normaMemOffset = (void*)offsetof(VertexProperty, normal);
	void* coordMemOffset = (void*)offsetof(VertexProperty, tCoord);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexProperty), positMemOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexProperty), normaMemOffset);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexProperty), coordMemOffset);

	glBindVertexArray(0);
}

void Shape::Render()
{
	//#NOTE: saving off vao, vbo and index -- only needs index. clean pls.
	glBindVertexArray(m_bufferInfo.m_VAO);
	glDrawElements(GL_TRIANGLES, m_bufferInfo.m_IndexCount, GL_UNSIGNED_INT, 0);
}

// Private functions:

void Shape::CreateShapeProperties(
	std::vector<VertexProperty>& a_verts,
	std::vector<unsigned int>& a_indices)
{
	//#TODO: FIX UP BINDINGS
	float mid = 1.0f / 2.0f;

	switch (m_shapeType)
	{
	case Shape::PLANE:
		// Front
		a_verts.push_back({ vec3(-1.0, -1.0, +0.0), vec3(+0.0, +0.0, +1.0), vec2(0.0, 0.0) });
		a_verts.push_back({ vec3(+1.0, -1.0, +0.0), vec3(+0.0, +0.0, +1.0), vec2(1.0, 0.0) });
		a_verts.push_back({ vec3(+1.0, +1.0, +0.0), vec3(+0.0, +0.0, +1.0), vec2(1.0, 1.0) });
		a_verts.push_back({ vec3(-1.0, +1.0, +0.0), vec3(+0.0, +0.0, +1.0), vec2(0.0, 1.0) });

		a_indices = { 0, 1, 2, 0, 2, 3 };

		break;
	case Shape::CUBE:
		// Front
		a_verts.push_back({ vec3(-mid, -mid, +mid), vec3(-1.0, -1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(+mid, -mid, +mid), vec3(+1.0, -1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(+mid, +mid, +mid), vec3(+1.0, +1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(-mid, +mid, +mid), vec3(-1.0, +1.0, +1.0), vec2() });
		// Right
		a_verts.push_back({ vec3(+mid, +mid, +mid), vec3(+1.0, +1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(+mid, +mid, -mid), vec3(+1.0, +1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, -mid, -mid), vec3(+1.0, -1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, -mid, +mid), vec3(+1.0, -1.0, +1.0), vec2() });
		// Back
		a_verts.push_back({ vec3(-mid, -mid, -mid), vec3(-1.0, -1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, -mid, -mid), vec3(+1.0, -1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, +mid, -mid), vec3(+1.0, +1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(-mid, +mid, -mid), vec3(-1.0, +1.0, -1.0), vec2() });
		// Left
		a_verts.push_back({ vec3(-mid, -mid, -mid), vec3(-1.0, -1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(-mid, -mid, +mid), vec3(-1.0, -1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(-mid, +mid, +mid), vec3(-1.0, +1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(-mid, +mid, -mid), vec3(-1.0, +1.0, -1.0), vec2() });
		// Back
		a_verts.push_back({ vec3(+mid, +mid, +mid), vec3(+1.0, +1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(-mid, +mid, +mid), vec3(-1.0, +1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(-mid, +mid, -mid), vec3(-1.0, +1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, +mid, -mid), vec3(+1.0, +1.0, -1.0), vec2() });
		// Bottom
		a_verts.push_back({ vec3(-mid, -mid, -mid), vec3(-1.0, -1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, -mid, -mid), vec3(+1.0, -1.0, -1.0), vec2() });
		a_verts.push_back({ vec3(+mid, -mid, +mid), vec3(+1.0, -1.0, +1.0), vec2() });
		a_verts.push_back({ vec3(-mid, -mid, +mid), vec3(-1.0, -1.0, +1.0), vec2() });

		a_indices = {
			 0,	 1,  2,  0,  2,  3, //front
			 4,	 5,  6,  4,  6,  7, //right
			 8,	 9, 10,  8, 10, 11, //back
			12, 13, 14, 12, 14, 15, //left
			16, 17, 18, 16, 18, 19, //upper
			20, 21, 22, 20, 22, 23  //bottom
		};

		break;
	case Shape::SPHERE:
		break;
	default:
		a_verts.push_back( {vec3(), vec3(), vec2() } );
		a_indices = { 0 };
		Console::Log(C_LOG_TYPE::LOG_ERROR, "Invalid shape type\n");
		break;
	}
}
