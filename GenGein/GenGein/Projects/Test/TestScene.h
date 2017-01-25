#pragma once

#include "..\BaseApp.h"

class SkyBox;
class GBuffer;
class ShaderSet;
class CamUB;
class OCMesh;
class ShadowFBO;

// WARNING: TEMP!

class TestScene : public BaseApp
{
public:
	TestScene();
	~TestScene();

	virtual void StartUp(const int, const int, const char*, bool = false);
	virtual void ShutDown();
	virtual void Update(const double);
	virtual void Render();
private:
	CamUB*		 m_pCamUB;
	SkyBox*		 m_pSkyBox;
	GBuffer*	 m_pGBuffer;
	ShaderSet*	 m_pShaders;
	OCMesh*		 m_pRuinsMesh;
};