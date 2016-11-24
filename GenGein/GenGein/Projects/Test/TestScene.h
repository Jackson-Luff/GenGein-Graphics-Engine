#pragma once

#include "..\BaseApp.h"

class UniBuff;
class OCMesh;
class SkyBox;
class ShaderSet;

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

	//void CreateIcosahedron();
private:
	ShaderSet* m_pShader;
	UniBuff* m_pCameraUniBuff;
	SkyBox* m_pSkyBox;

	OCMesh* m_pGroundMesh;
	OCMesh* m_pMonkeyMesh;	
};