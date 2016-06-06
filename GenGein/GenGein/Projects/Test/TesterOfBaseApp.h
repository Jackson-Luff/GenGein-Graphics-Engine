#pragma once

#include "Core\BaseApp.h"

class UniBuff;
class SProgram;
class GenMesh;

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
	UniBuff* m_pUniBuff;
	SProgram* m_pTestProgram;
	GenMesh* m_pGenMesh;
};