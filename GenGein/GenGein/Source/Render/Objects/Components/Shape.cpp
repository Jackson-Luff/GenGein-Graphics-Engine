#include "gl_core_4_4.h"

#include <vector>
#include <string>
#include <glm\glm.hpp>

#include "Input\Console\Console.h"
#include "Shape.h"

using C_LOG_TYPE = Console::LOG_TYPE;

void CreateCube(Shape::ShapeBufferProperties& a_shape)
{
	// clean up vertex handle for each object
	// remove multiple definitions of array

	glm::vec3 points[] = {
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, +1.0f, -1.0f },

		{ +1.0f, -1.0f, +1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ +1.0f, -1.0f, -1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ +1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },

		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ -1.0f, +1.0f, -1.0f },
		{ +1.0f, -1.0f, +1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ -1.0f, -1.0f, -1.0f },

		{ -1.0f, +1.0f, +1.0f },
		{ -1.0f, -1.0f, +1.0f },
		{ +1.0f, -1.0f, +1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ +1.0f, -1.0f, -1.0f },
		{ +1.0f, +1.0f, -1.0f },

		{ +1.0f, -1.0f, -1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ +1.0f, -1.0f, +1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, -1.0f },
		{ -1.0f, +1.0f, -1.0f },

		{ +1.0f, +1.0f, +1.0f },
		{ -1.0f, +1.0f, -1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, +1.0f, +1.0f },
		{ -1.0f, +1.0f, +1.0f },
		{ +1.0f, -1.0f, +1.0f },
	};

	a_shape.m_indexCount = 36;

	unsigned int vertFloatCount = a_shape.m_indexCount * sizeof(glm::vec3);
	glGenVertexArrays(1, &a_shape.m_VAO);
	glBindVertexArray(a_shape.m_VAO);

	glGenBuffers(1, &a_shape.m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, a_shape.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertFloatCount, &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);

	glBindVertexArray(0);
}

Shape::Shape()
{}

Shape::~Shape()
{
	glDeleteVertexArrays(1, &m_pProperties.m_VAO);
	glDeleteBuffers(1, &m_pProperties.m_VBO);
}

void Shape::Create(const ShapeType a_type)
{
	// #NOTE: clean up vertex handle for each object
	// remove multiple definitions of array

	switch (a_type)
	{
	case ShapeType::CUBE:
		CreateCube(m_pProperties);
		break;
	case ShapeType::SPHERE:

		break;
	default:
		Console::Log(C_LOG_TYPE::LOG_ERROR, "Invalid Shape Property \n");
		break;
	}
}

void Shape::Render()
{
	//#NOTE: saving off vao, vbo and index -- only needs index. clean pls.
	glBindVertexArray(m_pProperties.m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_pProperties.m_indexCount);
}