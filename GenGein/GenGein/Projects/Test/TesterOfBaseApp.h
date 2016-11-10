#pragma once

#include "Core\BaseApp.h"

class UniBuff;
class OCMesh;
class SkyBox;
class ShaderSet;

// WARNING: TEMP!
class TesterOfBaseApp : public BaseApp
{
public:
	TesterOfBaseApp();
	~TesterOfBaseApp();

	virtual void StartUp(const int, const int, const char*, bool = false);
	virtual void ShutDown();
	virtual void Update(const double);
	virtual void Render();

	//void CreateIcosahedron();
private:
	UniBuff* m_pCameraUniBuff;
	OCMesh* m_pTestOBJ;
	SkyBox* m_pSkyBox;
	ShaderSet* m_pShader;
	unsigned int* m_pObjectShader;
	unsigned int* m_pSkyShader;
};