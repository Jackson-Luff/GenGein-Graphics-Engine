#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "UniBuff.h"

UniBuff::UniBuff()
	: m_UBO(-1), m_blockIndex(-1),
	m_unibuffstorage(UNI_BUFF_STORAGE())
{}

UniBuff::~UniBuff()
{}

void UniBuff::SetUp(const glm::vec4& a_camPos, const glm::mat4& a_camProj, const glm::mat4& a_camView)
{
	//#TODO: set up uniform code to be like
	// addtounibuff(float*, float count, name)
	// 
	m_dataPointers.camPosition = &a_camPos;
	m_dataPointers.camProj = &a_camProj;
	m_dataPointers.camView = &a_camView;

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
		m_unibuffstorage.camProj != *m_dataPointers.camProj ||
		m_unibuffstorage.camView != *m_dataPointers.camView)
	{
		m_unibuffstorage.camPosition = *m_dataPointers.camPosition;
		m_unibuffstorage.camProj = *m_dataPointers.camProj;
		m_unibuffstorage.camView = *m_dataPointers.camView;

		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		void* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &m_unibuffstorage, sizeof(UNI_BUFF_STORAGE));
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		//#TODO: Allow multiple shader programs -- std::vector<programs> foreach
		unsigned int bindingPoint = 0;
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_UBO);
		//glUniformBlockBinding(*m_programID, m_blockIndex, bindingPoint);
	}
}