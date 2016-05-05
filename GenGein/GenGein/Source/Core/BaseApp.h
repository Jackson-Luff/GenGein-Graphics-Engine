#pragma once

class Time;
class GLwindow;
class BaseCam;

class BaseApp
{
public:
	BaseApp();
	~BaseApp();

	// Backbone methods
	virtual void StartUp(const int a_width, const int a_height, const char* a_title) = 0;
	virtual void ShutDown() = 0;
	virtual void Update(const double a_dt) = 0;
	virtual void Render() = 0;

	void Run();

protected:
	Time* m_pTime;
	GLwindow* m_pGLWindow;
	BaseCam* m_pBaseCam;

	double* m_pDT;
};

