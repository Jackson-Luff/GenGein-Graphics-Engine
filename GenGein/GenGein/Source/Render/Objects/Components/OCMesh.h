#pragma once
#include <vector>

//Can be both static mesh and rigged mesh
//Loads OBJ, FBX
class OCMesh
{
public:
	OCMesh();
	OCMesh(unsigned int a_programID);
	~OCMesh();

	bool Load(const char* a_filename, const char* a_baseDir);
	void OCMesh::Render();

	struct VertexBufferInfo
	{
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
		unsigned int m_index_count;
	};

private:
	unsigned int m_programID;

	std::vector<VertexBufferInfo> vertexBufferList;
};