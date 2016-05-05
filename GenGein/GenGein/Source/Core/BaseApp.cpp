#include <iostream>

#include "Input\Input.h"
#include "Input\Cameras\BaseCam.h"
#include "Console\Console.h"
#include "Render\GL\GLWindow.h"
#include "Time.h"

#include "BaseApp.h"

BaseApp::BaseApp()
{
}

BaseApp::~BaseApp()
{
	delete m_pTime;
	delete m_pGLWindow;
	delete m_pBaseCam;
}

void BaseApp::StartUp(const int a_width, const int a_height, const char* a_title)
{
	m_pTime = new Time(Time::DeltaTimeID::DT_FIXED);

	m_pGLWindow = new GLwindow();
	m_pGLWindow->SetUp(a_width, a_height, a_title);
	m_pGLWindow->SetWindowColour(0.2f, 0.65f, 0.75f);
	m_pGLWindow->EnableVSync(false);
	//m_pGLWindow->EnableFullScreen(true);
}

void BaseApp::ShutDown()
{
	m_pGLWindow->CleanUp();
}

// Private Functions:

void BaseApp::Run()
{
	/*__________Game Logic Circuit__________
	| GLFWwindow (?)						|
	|   |     							    |
	|	|--|T|--|-> Create Items and Camera.|
	|   |     | 						    |
	|   |     |-> GLFWwindow active (?)	    |
	|   |		   |						|
	|   |		   |-|T|-> Render & Update. |
	|  |F|		   |						|
	|	|		  |F|						|
	|	|		   |-> Shutdown App.		|
	|	|<---------|						|
	| Terminate							    |
	|_______________________________________*/

	// Game loop
	while (!m_pGLWindow->ShouldShutDown() && !Input::Keyboard::isKeyDown(KEY_ESCAPE))
	{
		double dt = m_pTime->GetDeltaTime();

		// Update:
		Input::Keyboard::RunDebugControls();
		// Update delta time
		m_pTime->Update();
		// Update camera
		m_pBaseCam->Update(dt);
		// Update mainframe
		Update(dt);

		// Clear window colour/depth buffers
		m_pGLWindow->ClearBuffers();
		// Render mainframe
		Render();
		// Swap front with back buffer
		m_pGLWindow->SwapBuffsPoleEvents();
	}

	// Shutdown mainframe
	ShutDown();
}
