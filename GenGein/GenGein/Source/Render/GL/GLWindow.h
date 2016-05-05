#pragma once

struct GLFWwindow; 

class GLwindow
{
public:
	GLwindow();
	~GLwindow();

	// Do before anything
	void SetUp(const int a_width, const int a_height, const char* a_title);
	// Clean in shutdown
	void CleanUp();
	// Changes window background colour
	void SetWindowColour(const float a_R, const float a_G, const float a_B);
	// Cleans colour and depth buffer
	void ClearBuffers();
	// Do after all rendering
	void SwapBuffsPoleEvents();
	// Consults GLFW for should close
	bool ShouldShutDown();


	// Enables/Disables VSYNC
	void EnableVSync(bool);
	// Enables Fullscreen
	void EnableFullScreen(bool);
private:

	bool InitGLWindow(const int a_width, const int a_height, const char* a_title);

	bool m_shouldClose;
	GLFWwindow* m_pWindow;
};

