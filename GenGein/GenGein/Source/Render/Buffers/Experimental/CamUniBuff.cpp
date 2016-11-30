#include "CamUniBuff.h"

#include <glm\glm.hpp>

CamUniBuff::CamUniBuff() : AdvUniBuff()
{}

CamUniBuff::CamUniBuff(unsigned int* a_programID) : AdvUniBuff()
{}

CamUniBuff::~CamUniBuff()
{
}

void CamUniBuff::SetUp(const glm::vec4& a_camPos, const glm::mat4& a_camProj, const glm::mat4& a_camView)
{
	//#TODO: set up uniform code to be like
	// addtounibuff(float*, float count, name)
	// 
	AddVec4ToVector(m_cameraInfo, a_camPos);
	AddMat4ToVector(m_cameraInfo, a_camProj);
	AddMat4ToVector(m_cameraInfo, a_camView);
	
	//for (const float* pData : m_cameraInfo)
	//	m_bufferStream.push_back(*pData);
	
	AdvUniBuff::SetUp();
}

void CamUniBuff::CleanUp()
{
	AdvUniBuff::CleanUp();
}

void CamUniBuff::Update()
{
	if(UpdateBuffer(m_bufferStream, m_cameraInfo))
		m_updateBuffer = true;

	AdvUniBuff::Update();
}
