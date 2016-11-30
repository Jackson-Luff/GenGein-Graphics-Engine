#include <glm\gtx\compatibility.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "AdvUniBuff.h"

AdvUniBuff::AdvUniBuff()
	: m_UBO(-1), m_blockIndex(-1)
{}

//AdvUniBuff::AdvUniBuff(unsigned int* a_programID) : AdvUniBuff()
//{
//	m_programID = a_programID;
//	m_blockIndex = glGetUniformBlockIndex(*m_programID, "CameraInfo");
//}

AdvUniBuff::~AdvUniBuff()
{}

void AdvUniBuff::SetUp()
{
	m_bufferSize = m_bufferStream.size() * sizeof(float);
	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, &m_bufferStream, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
}

void AdvUniBuff::CleanUp()
{
	glDeleteBuffers(1, &m_UBO);
}

void AdvUniBuff::Update()
{
	if (m_updateBuffer == GL_TRUE)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		void* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(p, &m_bufferStream, m_bufferSize);
		glUnmapBuffer(GL_UNIFORM_BUFFER);

		//#TODO: Allow multiple shader programs -- std::vector<programs> foreach
		unsigned int bindingPoint = 0;
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_UBO);
		//glUniformBlockBinding(*m_programID, m_blockIndex, bindingPoint);

		m_updateBuffer = GL_FALSE;
	}
}


void AdvUniBuff::AddVec4ToVector(std::vector<const float*>& a_out, const glm::vec4& a_in)
{
	for (int i = 0; i < 4; i++)
		a_out.push_back(&a_in[i]);
}

void AdvUniBuff::AddMat4ToVector(std::vector<const float*>& a_out, const glm::mat4& a_in)
{
	for (int i = 0; i < 16; i++)
	{
		int r = (int)(i / 4), c = (i % 4);
		a_out.push_back(&a_in[r][c]);
	}
}

bool AdvUniBuff::UpdateBuffer(std::vector<float>& a_stream, std::vector<const float*> a_refStream)
{
	bool result = false;
	for (size_t i = 0; i < a_refStream.size(); i++)
	{
		if (a_stream[i] != *a_refStream[i])
		{
			a_stream[i] = *a_refStream[i];
			result = true;
		}
	}

	return result;
}