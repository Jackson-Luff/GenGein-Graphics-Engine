#include "gl_core_4_4.h"
#include <glm\glm.hpp>

#include "Core\Time.h"	
#include "Console\Console.h"
#include "Input\Cameras\FlyCam.h"
#include "Render\Shaders\ShaderSet.h"
#include "Render\Objects\Components\OCMesh.h"
#include "Render\UniformBuffers\UniBuff.h"
#include "Render\SkyBox.h"


#include "TesterOfBaseApp.h"

TesterOfBaseApp::TesterOfBaseApp()
{
	Console::Log("#WAR | Remember to remove TesterOfBaseApp\n");
}

TesterOfBaseApp::~TesterOfBaseApp()
{
	delete m_pShader;
	delete m_pCameraUniBuff;
	delete m_pTestOBJ;
	delete m_pSkyBox;
}

void TesterOfBaseApp::StartUp(const int a_width, const int a_height, const char* a_title, bool a_fullscreen)
{
	////////////// SET UP BaseApp Prerequisites////////////////////////////////////
	// 
	BaseApp::StartUp(a_width, a_height, a_title, a_fullscreen);

	//Initialise camera
	FlyCam* pFlyCam = new FlyCam(glm::vec3(10, 0, 05), glm::vec3(0), 0.25, 0.2f);
	m_pBaseCam = pFlyCam;
	//
	///////////////////////////////////////////////////////////////////////////////
	
	m_pShader = new ShaderSet();
	m_pObjectShader = m_pShader->AddProgramFromExts({
		"Data/Shaders/Test/Test.vert",
		"Data/Shaders/Test/Test.frag"
	});
	m_pSkyShader = m_pShader->AddProgramFromExts({
		"Data/Shaders/Skybox/Skybox.vert",
		"Data/Shaders/Skybox/Skybox.frag"
	});

	// Set version of shaders
	m_pShader->SetVersion("450");
	//Set preamble to program AFTER 'version' definition m
	m_pShader->SetPreambleFile("Data/Shaders/preamble.glsl");
	m_pShader->UpdatePrograms();

	// 
	m_pSkyBox = new SkyBox(m_pSkyShader);
	m_pSkyBox->Create(SkyBox::SkyType::SKY);

	m_pCameraUniBuff = new UniBuff();
	m_pCameraUniBuff->SetUp(m_pBaseCam->GetPosition(), m_pBaseCam->GetProjTrans(), m_pBaseCam->GetViewTrans());

	// no normals == no mesh #TODO: CalcNormals() if none
	m_pTestOBJ = new OCMesh(*m_pObjectShader);
	m_pTestOBJ->Load("Data/Objects/toilet.obj","Data/Objects/");

}

void TesterOfBaseApp::ShutDown()
{
	BaseApp::ShutDown();
}

void TesterOfBaseApp::Update(const double a_dt)
{
	// Update GPU with Camera Updates:
	m_pCameraUniBuff->Update();
}

void TesterOfBaseApp::Render()
{
	// set up internal update (only update every x seconds)
	m_pShader->UpdatePrograms();

	glUseProgram(*m_pSkyShader);
	m_pSkyBox->Render();

	glUseProgram(*m_pObjectShader);
	m_pTestOBJ->Render();
}	