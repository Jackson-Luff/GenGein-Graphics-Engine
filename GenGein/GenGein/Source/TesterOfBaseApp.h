// WARNING: TEMP!
#pragma once
#include "Core\BaseApp.h"

class TesterOfBaseApp : public BaseApp
{
public:
	TesterOfBaseApp();
	~TesterOfBaseApp();

	virtual void StartUp(const int a_width, const int a_height, const char* a_title);
	virtual void ShutDown();
	virtual void Update(const double a_dt);
	virtual void Render();
private:
};