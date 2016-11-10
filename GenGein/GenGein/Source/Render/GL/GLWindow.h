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

	// Setup window  <width, height, name, fullscreen>
	void SetUp(const int, const int, const char*, bool = false);
	// Clean in shutdown
	void CleanUp();
	// Changes window background colour <r,g,b> range 0-1
	void SetWindowColour(const float, const float, const float);
	// Attempts to re-size window if window has been scaled
	void AttemptWindowReSize();
	// Cleans colour and depth buffer
	void ClearBuffers();
	// Do after all rendering
	void SwapBuffsPoleEvents();
	// Consults GLFW for should close
	bool ShouldShutDown();

	// Enable Depth Testing
	void EnableDepthTest(bool a_enable);
	// Transparent handle
	void EnableOneMinusAlphaBlend(bool a_enable);
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
	int m_windowWidth, m_windowHeight;

	GLFWwindow* m_pWindow;
};

