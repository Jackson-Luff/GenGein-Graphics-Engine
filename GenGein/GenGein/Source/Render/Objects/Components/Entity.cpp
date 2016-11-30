
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "gl_core_4_4.h"

#include "Entity.h"

using namespace glm;

Entity::Entity()
{
	m_LocalTrans = mat4();
	m_locMatUniLoc = -1;
}

Entity::Entity(unsigned int* a_progID)
	: Entity()
{
	m_programID = a_progID;
	m_locMatUniLoc = glGetUniformLocation(*m_programID, "LocalMatrix");
}

Entity::Entity(unsigned int* a_progID, const vec4 a_pos) 
	: Entity(a_progID)
{
	SetPosition(a_pos);
}

Entity::Entity(unsigned int* a_progID, const vec4 a_pos, const mat3 a_rot, const vec3 a_scale) 
	: Entity(a_progID)
{
	SetPosition(a_pos);
	SetRotation(a_rot);
	SetScale(a_scale);
}


Entity::~Entity()
{
}

void Entity::Update()
{
	//Update the uniform buffer for the local matrix
	glUniformMatrix4fv(m_locMatUniLoc, 1, GL_FALSE, &m_LocalTrans[0][0]);

	//#TODO!
	//if(*m_pLocalTrans != m_LocalTrans)
	//{
	//	// Update the pointer with transform
	//
	//	// As well as update uniform buffer with 
	//	// the new transform
	//	glUniformMatrix4fv(m_locMatUniLoc, 1, GL_FALSE, &m_LocalTrans[0][0]);
	//}
}

// Sets

const bool Entity::SetScale(const vec3 a_nScale)
{
	m_LocalTrans = scale(m_LocalTrans, a_nScale);
	return true;
}

const bool Entity::SetPosition( const vec4 a_nPos )
{
	m_LocalTrans[3] = a_nPos;
	return true;
}

const bool Entity::SetRotation(const mat3 a_nRot)
{
	vec4 position = m_LocalTrans[3];
	mat4 rotMat = mat4(a_nRot);
	rotMat[3] = position;

	m_LocalTrans = rotMat;
	return true;
}

const bool Entity::SetTransform(const mat4 a_nRot)
{
	m_LocalTrans = a_nRot;
	return true;
}


// Adds

const bool Entity::AddScale(const vec3 a_nScale)
{
	mat3 scaledMatrix = mat3(m_LocalTrans);
	vec3 s = vec3(length(scaledMatrix[0]), length(scaledMatrix[1]), length(scaledMatrix[2]));
	s += a_nScale;

	m_LocalTrans = scale(m_LocalTrans, s);
	return true;
}

const bool Entity::AddPosition(const vec3 a_nPos)
{
	m_LocalTrans[3] += vec4(a_nPos, 0.0);
	return true;
}

const bool Entity::AddRotation(const float a_nAngle, const vec3 a_nAxis)
{
	m_LocalTrans = rotate(m_LocalTrans, a_nAngle, a_nAxis);
	return true;
}

const bool Entity::AddTransform(const mat4 a_nRot)
{
	//#TODO: clean combination via glm?
	m_LocalTrans += a_nRot;
	return true;
}