#include "Core\Time.h"
#include "Core\GL\GLWindow.h"

#include "Input\Console\Console.h"
#include "Input\Cameras\FlyCam.h"
#include "Render\Shaders\ShaderSet.h"
#include "Render\Objects\Components\OCMesh.h"
#include "Render\Buffers\GBuffer.h"
#include "Render\Buffers\CamUB.h"
#include "Render\SkyBox.h"
#include "Render\Textures\Texture2D.h"

#include "TestScene.h"

using C_LOG_TYPE = Console::LOG_TYPE;

TestScene::TestScene() : BaseApp()
{
	Console::Log(C_LOG_TYPE::LOG_WARNING, "Remember to remove TestScene\n");
}

TestScene::~TestScene()
{
	delete m_pCamUB;
	delete m_pGBuffer;
	delete m_pMonkeyMesh;
	delete m_pGroundMesh;
	delete m_pShaders;
	delete m_pSkyBox;
}

void TestScene::StartUp(const int a_width, const int a_height, const char* a_title, bool a_fullscreen)
{
	////////////// SET UP BaseApp Prerequisites////////////////////////////////////
	//
	// Init base
	BaseApp::StartUp(a_width, a_height, a_title, a_fullscreen);
	// Init camera
	m_pBaseCam = new FlyCam(glm::vec3(10, 0, 5), glm::vec3(0), 0.15f, 0.2f);
	//
	///////////////////////////////////////////////////////////////////////////////

	int winWidth  = m_pGLWindow->GetWidth();
	int winHeight = m_pGLWindow->GetHeight();

	m_pShaders = new ShaderSet();

	unsigned int* pSkyBox = m_pShaders->AddProgramFromExts({
		"_Resources/Shaders/Skybox/Skybox.vert",
		"_Resources/Shaders/Skybox/Skybox.frag"
	});
	unsigned int* pGBuffer = m_pShaders->AddProgramFromExts({
		"_Resources/Shaders/Test/GBuffer.vert",
		"_Resources/Shaders/Test/GBuffer.frag"
	});
	unsigned int* pGPost = m_pShaders->AddProgramFromExts({
		"_Resources/Shaders/Test/PostQuad.vert",
		"_Resources/Shaders/Test/PostQuad.frag"
	});

	// Set version of shaders
	m_pShaders->SetVersion("450");
	//Set preamble to program AFTER 'version' definition 
	m_pShaders->UpdatePrograms();
	
	//
	m_pCamUB = new CamUB();
	m_pCamUB->SetUp(m_pBaseCam->GetWorldTrans(), m_pBaseCam->GetProjTrans(), m_pBaseCam->GetViewTrans());

	//
	m_pGBuffer = new GBuffer(pGPost);
	m_pGBuffer->SetUp(winWidth, winHeight);

	// 
	m_pSkyBox = new SkyBox(pSkyBox);
	m_pSkyBox->Create(SkyBox::SkyType::MORNING);

	// no normals == no mesh #TODO: CalcNormals() if none
	m_pGroundMesh = new OCMesh(pGBuffer, glm::vec4(0, -2, 0, 1));
	m_pGroundMesh->Load("_Resources/Objects/ground.obj", "_Resources/Objects/");

	for (int i = 0; i < 9; i++)
	{
		int r = (i / 3) * 10 - 10;
		int c = (i % 3) * 10 - 10;
		m_pGroundMesh->AddInstance(glm::vec4(r, -2, c, 1));
	}
	m_pGroundMesh->AddInstance(glm::vec4(0, 2, 0, 1));
	m_pMonkeyMesh = new OCMesh(pGBuffer);
	m_pMonkeyMesh->Load("_Resources/Objects/monkey.obj", "_Resources/Objects/");
}

void TestScene::ShutDown()
{
	BaseApp::ShutDown();
}

void TestScene::Update(const double a_dt)
{
	// Update GPU with Camera Updates:
	m_pCamUB->Update();

	m_pMonkeyMesh->AddRotation( (float)a_dt/50.0f, glm::vec3(0, 1, 0) );
}

void TestScene::Render()
{
	// #TODO: set up internal update (only update every x seconds)
	m_pShaders->UpdatePrograms();

	m_pGBuffer->BindForWriting();
	m_pGLWindow->ClearBuffers();
	
	m_pSkyBox->Render();

	m_pMonkeyMesh->Render();
	m_pGroundMesh->Render();
	
	m_pGBuffer->BindForReading();
	m_pGBuffer->Render();
}