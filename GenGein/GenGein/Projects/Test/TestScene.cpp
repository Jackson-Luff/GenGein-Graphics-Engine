#include "TestScene.h"

#include "Core\Time.h"
#include "Core\GL\GLWindow.h"

#include "Input\Console\Console.h"
#include "Input\Cameras\FlyCam.h"
#include "Render\Shaders\ShaderSet.h"
#include "Render\Objects\Components\OCMesh.h"
#include "Render\Buffers\GBuffer.h"
#include "Render\Buffers\CamUB.h"
#include "Render\SkyBox.h"

TestScene::TestScene() : BaseApp()
{
	Console::Log(Console::FBACK::LOG_WARNING, "Remember to remove TestScene\n");
}

TestScene::~TestScene()
{
	delete m_pCamUB;
	delete m_pGBuffer;
	delete m_pRuinsMesh;
	delete m_pShaders;
	delete m_pSkyBox;
}

void TestScene::StartUp(const int a_width, const int a_height, const char* a_title, bool a_fullscreen)
{
	// #NOTE: Clean this it's flithy

	////////////// SET UP BaseApp Prerequisites////////////////////////////////////
	//
	// Init base
	BaseApp::StartUp(a_width, a_height, a_title, a_fullscreen);
	// Init camera
	m_pBaseCam = new FlyCam(glm::vec3(0, 10, 10), glm::vec3(0), 0.30f, 0.2f);
	//
	///////////////////////////////////////////////////////////////////////////////

	int winWidth  = m_pGLWindow->GetWidth();
	int winHeight = m_pGLWindow->GetHeight();

	m_pShaders = new ShaderSet();

	// --------------------- SKYBOX SHADER(S) --------------------------

	unsigned int* pSkyBox		= m_pShaders->AddProgramFromExts({
		"Skybox/Skybox.vert", "Skybox/Skybox.frag"	});

	// --------------------- OBJECT SHADER(S) --------------------------

	unsigned int* pGB_TestObj	= m_pShaders->AddProgramFromExts({
		"Test/GB_Object.vert", "Test/GB_Object.frag" });

	// --------------------- GBUFFER SHADER(S) --------------------------

	unsigned int* pGB_Comp		= m_pShaders->AddProgramFromExts({
		"Test/GB_Composite.vert", "Test/GB_Composite.frag"	});

	unsigned int* pPostProcess	= m_pShaders->AddProgramFromExts({
		"Test/PostProcess.vert", "Test/PostProcess.frag" });

	// Set version of shaders
	m_pShaders->SetVersion("450");
	//Set preamble to program AFTER 'version' definition 
	m_pShaders->UpdatePrograms();
	
	// 
	m_pSkyBox = new SkyBox(pSkyBox);
	m_pSkyBox->Create(SkyBox::SkyTheme::MORNING);

	// no normals == no mesh #TODO: CalcNormals() if none
	m_pRuinsMesh = new OCMesh(pGB_TestObj, glm::vec4(1));
	m_pRuinsMesh->Load("_Resources/Objects/Scene/Ruins.obj", "_Resources/Objects/");

	//
	m_pGBuffer = new GBuffer(pGB_Comp);
	m_pGBuffer->SetUp(winWidth, winHeight);

	//
	m_pCamUB = new CamUB();
	m_pCamUB->SetUp(m_pBaseCam->GetWorldTrans(), m_pBaseCam->GetProjTrans(), m_pBaseCam->GetViewTrans());
}

void TestScene::ShutDown()
{
	BaseApp::ShutDown();
}

void TestScene::Update(const double a_dt)
{
	// Update GPU with Camera Updates:
	m_pCamUB->Update();

	//m_pMonkeyMesh->AddRotation( (float)a_dt/50.0f, glm::vec3(0, 1, 0) );
}

void TestScene::Render()
{
	// #TODO: set up internal update (only update every x seconds)
	m_pShaders->UpdatePrograms();

	m_pGBuffer->BindForWriting();
	
	m_pGLWindow->ClearBuffers();
	
	m_pSkyBox->Render();
	m_pRuinsMesh->Render();
	
	m_pGBuffer->BindForReading();
	m_pGLWindow->ClearBuffers();

	m_pGBuffer->Render();
}