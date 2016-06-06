#pragma once

struct GLFWwindow;

class GLwindow
{
public:
	enum MONITOR_ID
	{
		PRIMARY,
		SECONDARY
	};

	GLwindow();
	~GLwindow();

	// Do before anything
	void SetUp(const int, const int, const char*, bool = false);
	// Clean in shutdown
	void CleanUp();
	// Changes window background colour
	void SetWindowColour(const float, const float, const float);
	// Cleans colour and depth buffer
	void ClearBuffers();
	// Do after all rendering
	void SwapBuffsPoleEvents();
	// Consults GLFW for should close
	bool ShouldShutDown();


	// Enables/Disables VSYNC
	void EnableVSync(bool);
	// Enables Fullscreen
	void EnableFullscreen(bool);
	// Select Monitor to use
	void SelectMonitor(MONITOR_ID);
private:

	bool InitGLWindow(const int, const int, const char*, bool);

	bool m_shouldClose;
	bool m_fullscreen;
	GLFWwindow* m_pWindow;
};

