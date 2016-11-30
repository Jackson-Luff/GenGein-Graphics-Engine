#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "CamUB.h"

CamUB::CamUB()
	: m_UBO(-1), m_blockIndex(-1),
	m_unibuffstorage(UNI_BUFF_STORAGE())
{}

CamUB::~CamUB()
{}

void CamUB::SetUp(const glm::mat4& a_camWlrd, const glm::mat4& a_camProj, const glm::mat4& a_camView)
{
	//#TODO: set up uniform code to be like
	// addtoCamUB(float*, float count, name)
	// 
	m_dataPointers.camWorld = &a_camWlrd;
	m_dataPointers.camProj = &a_camProj;
	m_dataPointers.camView = &a_camView;

	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UNI_BUFF_STORAGE), &m_unibuffstorage, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

void CamUB::CleanUp()
{
	glDeleteBuffers(1, &m_UBO);
}

void CamUB::Update()
{
	// #NOTE: Not that efficient. Comparing 3 matricies.. Jesus.
	// #TODO: Improve this process
	if (m_unibuffstorage.camWorld != *m_dataPointers.camWorld ||
		m_unibuffstorage.camProj  != *m_dataPointers.camProj  ||
		m_unibuffstorage.camView  != *m_dataPointers.camView)
	{
		m_unibuffstorage.camWorld = *m_dataPointers.camWorld;
		m_unibuffstorage.camProj = *m_dataPointers.camProj;
		m_unibuffstorage.camView = *m_dataPointers.camView;

		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		void* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &m_unibuffstorage, sizeof(UNI_BUFF_STORAGE));
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		//#TODO: Allow multiple shader programs -- std::vector<programs> foreach
		unsigned int bindingPoint = 0;
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_UBO);
	}
}