#pragma once
#include <glm\fwd.hpp>

class GenMesh
{
public:
	GenMesh();
	~GenMesh();

	struct VertexData
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 uv;
	};

	void GenSegmentedGrid(const unsigned int, const float, bool = false, bool = false);
	void GenQuad(const glm::vec4, const float);

	void Render();

private:
	// Send Vertex data to Buffers:
	void ApplyDataToGL(const unsigned int* a_indices);

	// Base Mods:
	void AddPerlinToGUI();
	void GenPerlin();
	void GenDimSqr();

	// Base data for outcome:
	VertexData* verts;
	unsigned int m_indexCount;
	unsigned int m_VAO, m_VBO, m_IBO;

	// Perlin private members:
	unsigned int m_dims;
	float m_persistence;
	float m_amplitude;
	float m_octaves;
	float m_seeder;
};

