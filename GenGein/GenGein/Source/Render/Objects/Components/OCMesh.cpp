// Dependancy includes
#include "tinyOBJ\tiny_obj_loader.h"

#include "glm\glm.hpp"
#include "gl_core_4_4.h"

// Engine includes
#include "Console\Console.h"
#include "OCMesh.h"

using namespace glm;

typedef std::vector<tinyobj::shape_t> Shape_t;
typedef std::vector<tinyobj::material_t> Material_t;

OCMesh::OCMesh()
{
}

OCMesh::OCMesh(unsigned int a_programID)
{
	m_programID = a_programID;
}

OCMesh::~OCMesh()
{
	for (VertexBufferInfo& buffer : vertexBufferList)
	{
		glDeleteVertexArrays(1, &buffer.m_VAO);
		glDeleteBuffers(1, &buffer.m_VBO);
		glDeleteBuffers(1, &buffer.m_IBO);
	}
	vertexBufferList.clear();
}

/*
float* CalcNormal(float v0[3], float v1[3], float v2[3]) {
	
	float v10[3], v20[3], n[3];

	for (int i = 0; i < 3; i++)
	{
		v10[i] = v1[i] - v0[i];
		v20[i] = v2[i] - v0[i];
	}

	n[0] = v20[1] * v10[2] - v20[2] * v10[1];
	n[1] = v20[2] * v10[0] - v20[0] * v10[2];
	n[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = (n[0] * n[0]) + (n[1] * n[1]) + (n[2] * n[2]);
	if (len2 > 0.0f) {
		float len = sqrtf(len2);

		n[0] /= len;
		n[1] /= len;
	}

	return n;
}

float* Calc_Tangents_BiNormals_Normals(tinyobj::mesh_t& a_m)
{
	for(unsigned int v = 0; v < (a_m.positions.size() / 3); v++)
	{
		float v0[3] = { a_m.positions[3*v+0], a_m.positions[3*v+1], a_m.positions[3*v+2]};
		float v1[3] = { a_m.positions[3*v+3], a_m.positions[3*v+4], a_m.positions[3*v+5]};
		float v2[3] = { a_m.positions[3*v+6], a_m.positions[3*v+7], a_m.positions[3*v+8]};

	}
}
*/
std::vector<OCMesh::VertexBufferInfo> CreateOpenGLBuffers( std::vector<tinyobj::shape_t>& a_shapes)
{
	std::vector<OCMesh::VertexBufferInfo> buffer_List;
	buffer_List.resize(a_shapes.size());

	for (tinyobj::shape_t& s : a_shapes)
	{
		OCMesh::VertexBufferInfo buffInfo;

		glGenVertexArrays(1, &buffInfo.m_VAO);
		glBindVertexArray(buffInfo.m_VAO);
		glGenBuffers(1, &buffInfo.m_VBO);
		glGenBuffers(1, &buffInfo.m_IBO);

		unsigned int pos_count = (unsigned int)s.mesh.positions.size();
		unsigned int nor_count = (unsigned int)s.mesh.normals.size();
		unsigned int tex_count = (unsigned int)s.mesh.texcoords.size();

		std::vector<float> v_data;
		v_data.reserve(pos_count + nor_count + tex_count);

		v_data.insert(v_data.end(), s.mesh.positions.begin(), s.mesh.positions.end());
		v_data.insert(v_data.end(), s.mesh.normals.begin(), s.mesh.normals.end());
		v_data.insert(v_data.end(), s.mesh.texcoords.begin(), s.mesh.texcoords.end());

		//for (size_t i = 0; i < s.mesh.positions.size(); i++)
		//{
		//	float n[3] = CalcNormal(s.mesh.positions[3 * );
		//	v_data.push_back(n[0]);
		//	v_data.push_back(n[1]);
		//	v_data.push_back(n[2]);
		//}

		//Index data
		buffInfo.m_index_count = (unsigned int)s.mesh.indices.size();

		glBindBuffer(GL_ARRAY_BUFFER, buffInfo.m_VBO);
		glBufferData(GL_ARRAY_BUFFER, v_data.size() * sizeof(float), v_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffInfo.m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			buffInfo.m_index_count * sizeof(unsigned int), s.mesh.indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normal data
		glEnableVertexAttribArray(2); //tex coord data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * pos_count));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * nor_count));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		buffer_List.push_back(buffInfo);
	}

	return buffer_List;
}

bool OCMesh::Load(const char* a_filename, const char* a_baseDir)
{
	
	std::string err;
	Shape_t shapes;
	Material_t materials;

	bool ret = tinyobj::LoadObj(shapes, materials, err, a_filename, a_baseDir);
	
	if (!err.empty()) {
		Console::Log("#WAR | %s \n", err.c_str() );
	}

	if (!ret) {
		Console::Log("#ERR | %s \n","FAILED TO LOAD/PARSE .OBJ\n");
		return false;
	}

	vertexBufferList = CreateOpenGLBuffers(shapes);

	return true;
}

void OCMesh::Render()
{
	//glUseProgram(m_programID);

	for (VertexBufferInfo& buffer : vertexBufferList) 
	{
		glBindVertexArray(buffer.m_VAO);
		glDrawElements(GL_TRIANGLES, buffer.m_index_count, GL_UNSIGNED_INT, 0);
	}
}

