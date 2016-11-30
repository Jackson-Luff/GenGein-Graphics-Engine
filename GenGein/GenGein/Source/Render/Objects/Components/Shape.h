#pragma once
#include <vector>
#include <glm\glm.hpp>

class Shape
{
public:

	enum ShapeType : char
	{
		PLANE,
		CUBE,
		SPHERE
	};

	Shape();
	~Shape();

	void Create(const ShapeType a_type);
	void Render();

	struct VertexProperty
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tCoord;
	};
	
private:

	struct BufferProperties
	{
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
		unsigned int m_IndexCount;
	};

	//
	void CreateShapeProperties(
		std::vector<VertexProperty>& a_verts,
		std::vector<unsigned int>& a_indices);
	
	ShapeType m_shapeType;
	BufferProperties m_bufferInfo;
};
