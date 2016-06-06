#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "UniBuff.h"

UniBuff::UniBuff(unsigned int a_programID)
	: m_UBO(-1), m_blockIndex(-1),
	m_programID(a_programID),
	m_unibuffstorage(UNI_BUFF_STORAGE())
{}

UniBuff::~UniBuff()
{}

void UniBuff::SetUp(const glm::vec4& a_camPos, const glm::mat4& a_camProjView)
{
	m_dataPointers.camPosition = &a_camPos;
	m_dataPointers.camProjView = &a_camProjView;

	m_blockIndex = glGetUniformBlockIndex(m_programID, "UniBuff");

	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UNI_BUFF_STORAGE), &m_unibuffstorage, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

void UniBuff::CleanUp()
{
	glDeleteBuffers(1, &m_UBO);
}

void UniBuff::Update()
{
	if (m_unibuffstorage.camPosition != *m_dataPointers.camPosition ||
		m_unibuffstorage.camProjView != *m_dataPointers.camProjView)
	{
		m_unibuffstorage.camPosition = *m_dataPointers.camPosition;
		m_unibuffstorage.camProjView = *m_dataPointers.camProjView;

		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		void* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &m_unibuffstorage, sizeof(UNI_BUFF_STORAGE));
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		unsigned int bindingPoint = 0;
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_UBO);
		glUniformBlockBinding(m_programID, m_blockIndex, bindingPoint);
	}
}