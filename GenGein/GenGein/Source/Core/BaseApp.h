#pragma once

class Time;
class UniBuff;
class GLwindow;
class BaseCam;

class BaseApp
{
public:
	BaseApp();
	~BaseApp();

	// Backbone methods
	virtual void StartUp(const int, const int, const char*, bool = false) = 0;
	virtual void ShutDown() = 0;
	virtual void Update(const double) = 0;
	virtual void Render() = 0;

	void Run();

protected:
	Time* m_pTime;
	GLwindow* m_pGLWindow;
	BaseCam* m_pBaseCam;
};
