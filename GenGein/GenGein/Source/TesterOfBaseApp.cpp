#include <iostream>

#include "Render\Gizmos.h"
#include "Input\Cameras\FlyCam.h"

#include "TesterOfBaseApp.h"

#include <gl_core_4_4.h>

TesterOfBaseApp::TesterOfBaseApp()
{
	printf("Remember to remove TesterOfBaseApp\n");
}

TesterOfBaseApp::~TesterOfBaseApp()
{
}

void TesterOfBaseApp::StartUp(const int a_width, const int a_height, const char* a_title)
{
	BaseApp::StartUp(a_width, a_height, a_title);
	
	//Initialise camera
	FlyCam* pFlyCam = new FlyCam(glm::vec3(0,0,0), glm::vec3(0,0,10), 2, 0.3f);
	m_pBaseCam = pFlyCam;

	Gizmos::create();

}

void TesterOfBaseApp::ShutDown()
{
	Gizmos::destroy();

	BaseApp::ShutDown();
}

void TesterOfBaseApp::Update(const double a_dt)
{}

void TesterOfBaseApp::Render()
{
	Gizmos::clear();

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Gizmos::addSphere(glm::vec3(3 *i,0, 3*j), 1, 5, 5, glm::vec4(1));
		}
	}

	Gizmos::draw(m_pBaseCam->GetProjViewTrans());
}