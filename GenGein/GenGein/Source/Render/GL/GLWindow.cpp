#include <iostream>
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include "GLwindow.h"

void error_callback(int a_error, const char* a_description)
{
	// Prints the error to console.
	printf("%i  %s\n", a_error, a_description);
}

GLwindow::GLwindow()
	: m_pWindow(glfwCreateWindow(800, 600, "DEFAULT_NON_ASSIGNED", 0, 0))
{}

GLwindow::~GLwindow()
{
	
}

void GLwindow::SetUp(const int a_width, const int a_height, const char* a_title)
{
	if (InitGLWindow(a_width, a_height, a_title))
		printf("SUCCESS: Window Build Complete!\n");
	else
		return (void)printf("ERROR: Window Build Failed!\n");
}

void GLwindow::CleanUp()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void GLwindow::SetWindowColour(const float a_R, const float a_G, const float a_B)
{
	if ((a_R < 0 || a_R > 1) ||
		(a_G < 0 || a_G > 1) ||
		(a_B < 0 || a_B > 1))
		printf("WARNING: window colour exceeds GL units.\n");

	glClearColor(a_R, a_G, a_B, 1.0f);
}

void GLwindow::ClearBuffers()
{
	// Clear colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLwindow::SwapBuffsPoleEvents()
{
	// Swap Buffers of Window
	glfwSwapBuffers(m_pWindow);
	// Poll Various Events (E.g Keyboard)
	glfwPollEvents();
}

bool GLwindow::ShouldShutDown()
{
	if (glfwWindowShouldClose(m_pWindow))
		return true;

	return false;
}

void GLwindow::EnableVSync(bool a_sync)
{
	// Function pointer for the wgl extention function we need to enable/disable
	// vsync
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString(GL_EXTENSIONS);

	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
		return;
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(a_sync);
	}
}

void GLwindow::EnableFullScreen(bool)
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);


	//##BIGNOTE: STILL VERY MUCH IN THE WORKS
	//if (m_pWindow != NULL)
	//	glfwDestroyWindow(m_pWindow);

	m_pWindow = glfwCreateWindow(mode->width, mode->height, "My Title", monitor, NULL);
	glViewport(0, 0, mode->width, mode->height);
}

// Private Functions:

bool GLwindow::InitGLWindow(const int a_width, const int a_height, const char* a_title)
{
	// If approaching error, this will print it out
	glfwSetErrorCallback(error_callback);

	// Check: is GLFWinit safe?
	if (!glfwInit())
		return false;

	// Create Window knowing the init is safe
	m_pWindow = glfwCreateWindow(a_width, a_height, a_title, 0, 0);

	// Check: is the window safe?
	if (m_pWindow == nullptr) {
		glfwTerminate();
		return false;
	}

	// Now we know the window is safe; apply context.
	glfwMakeContextCurrent(m_pWindow);
	
	//// OpenGL Loader Generator
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	// All's a-go. 
	return true;
}

