#pragma once
#include <vector>
#include "Entity.h"

// #TODO: When buffer handle is generic, 
// remove this kid
#include <tinyOBJ\tiny_obj_loader.h>

//Can be both static mesh and rigged mesh
//Loads OBJ, FBX
class OCMesh : public Entity
{
public:
	OCMesh();
	OCMesh(unsigned int* a_programID);
	OCMesh(unsigned int* a_programID, glm::vec4 a_pos);
	~OCMesh();

	//
	bool Load(const char* a_filename, const char* a_baseDir);
	//
	void Render();

	//
	void UseProgram();
	//
	void AddInstance(glm::vec4 a_pos);

private:
	
	//#TODO: handle buffers more generically in ENTITY - NOT HERE!
	std::vector<VertexBufferInfo> CreateOpenGLBuffers(std::vector<tinyobj::shape_t>& a_shapes);
	std::vector<VertexBufferInfo> vertexBufferList;

	//#TODO: USE GL_INSTANCE METHOD. TEMP
	// VERY LIMITED (FOR NOW)
	std::vector<glm::vec4> m_instances;
};