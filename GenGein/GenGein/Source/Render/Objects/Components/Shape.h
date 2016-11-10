#pragma once

#include <vector>
#include <string>

class Shape
{
public:

	enum ShapeType : char
	{
		CUBE,
		SPHERE
	};

	Shape();
	~Shape();

	void Create(const ShapeType a_type);
	void Render();

	struct ShapeProperties
	{
		glm::vec3* positions;
		glm::vec3* normals;
		glm::vec2* texCoords;
	};

	struct ShapeBufferProperties
	{
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_indexCount;
	};

private:
	
	ShapeBufferProperties m_pProperties;
};
