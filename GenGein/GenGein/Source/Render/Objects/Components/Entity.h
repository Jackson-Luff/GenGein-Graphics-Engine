#pragma once
#include <glm\glm.hpp>

class Entity
{
public:
	Entity();
	Entity(unsigned int* a_prog);
	Entity(unsigned int* a_prog, const glm::vec4 a_pos);
	Entity(unsigned int* a_prog, const glm::vec4 a_pos, const glm::mat3 a_rot, const glm::vec3 a_scale);
	~Entity();
	
	void Update();

	// General orientation GETs
	inline const glm::vec4& GetPosition()  const { return m_LocalTrans[3]; }
	inline const glm::mat3& GetRotation()  const { return glm::mat3(m_LocalTrans); }
	inline const glm::mat4& GetTransform() const { return m_LocalTrans; }

	inline const glm::vec4& GetForwardVector() const { return m_LocalTrans[2]; }

	// General orientation SETs
	const bool SetScale(const glm::vec3 a_nScale);
	const bool SetPosition(const glm::vec4 a_nPos);
	const bool SetRotation(const glm::mat3 a_nRot);
	const bool SetTransform(const glm::mat4 a_nTrans);

	// General orientation ADDs
	const bool AddScale(const glm::vec3 a_nScale);
	const bool AddPosition(const glm::vec3 a_nPos);
	const bool AddRotation(const float a_nAngle, const glm::vec3 a_nAxis);
	const bool AddTransform(const glm::mat4 a_nTrans);

protected:

	struct VertexBufferInfo
	{
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
		unsigned int m_index_count;
	};

	unsigned int* m_programID;
	glm::mat4 m_LocalTrans;

private:
	// #TODO:
	// Point to local trans = 
	// only update when needed
	//glm::mat4* m_pLocalTrans;
	int m_locMatUniLoc;
};

