#pragma once
#include <glm\fwd.hpp>

#include "AdvUniBuff.h"
#include <vector>

class CamUniBuff : public AdvUniBuff
{
public:
	CamUniBuff();
	CamUniBuff(unsigned int* a_programID);
	~CamUniBuff();

	virtual void SetUp(const glm::vec4& a_camPos, const glm::mat4& a_camProj, const glm::mat4& a_camView);
	virtual void CleanUp();

	virtual void Update();

private:

	std::vector<const float*> m_cameraInfo;
	/*
	struct CAMERA_REFERENCE
	{
		const float* _cameraInfo[4];
	} m_cameraPointers;

	struct STORED_CAMERA_INFO
	{
		float _cameraPosition[4];
		float _cameraProjection[4][4];
		float _cameraView[4][4];
	} m_storedInfo;*/
};