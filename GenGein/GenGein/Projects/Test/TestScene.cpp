#include "Core\Time.h"
#include "Input\Console\Console.h"
#include "Input\Cameras\FlyCam.h"
#include "Render\Shaders\ShaderSet.h"
#include "Render\Objects\Components\OCMesh.h"
#include "Render\UniformBuffers\UniBuff.h"
#include "Render\SkyBox.h"

#include "TestScene.h"

using C_LOG_TYPE = Console::LOG_TYPE;

TestScene::TestScene()
{
	Console::Log(C_LOG_TYPE::LOG_WARNING, "Remember to remove TestScene\n");
}

TestScene::~TestScene()
{
	delete m_pShader;
	delete m_pCameraUniBuff;
	delete m_pMonkeyMesh;
	delete m_pGroundMesh;
	delete m_pSkyBox;
}

void TestScene::StartUp(const int a_width, const int a_height, const char* a_title, bool a_fullscreen)
{
	////////////// SET UP BaseApp Prerequisites////////////////////////////////////
	// 
	// Init base
	BaseApp::StartUp(a_width, a_height, a_title, a_fullscreen);
	// Init camera
	m_pBaseCam = new FlyCam(glm::vec3(10, 0, 5), glm::vec3(0), 0.25, 0.2f);
	//
	///////////////////////////////////////////////////////////////////////////////

	m_pShader = new ShaderSet();
	unsigned int* pCube = m_pShader->AddProgramFromExts({
		"_Resources/Shaders/Skybox/Skybox.vert",
		"_Resources/Shaders/Skybox/Skybox.frag"
	});
	unsigned int* pMonkey = m_pShader->AddProgramFromExts({
		"_Resources/Shaders/Test/Monkey.vert",
		"_Resources/Shaders/Test/Monkey.frag"
	});
	unsigned int* pGround = m_pShader->AddProgramFromExts({
		"_Resources/Shaders/Test/Ground.vert",
		"_Resources/Shaders/Test/Ground.frag"
	});
	
	// Set version of shaders
	m_pShader->SetVersion("450");
	//Set preamble to program AFTER 'version' definition 
	m_pShader->SetPreambleFile("_Resources/Shaders/preamble.glsl");
	m_pShader->UpdatePrograms();

	m_pCameraUniBuff = new UniBuff();
	m_pCameraUniBuff->SetUp(m_pBaseCam->GetPosition(), m_pBaseCam->GetProjTrans(), m_pBaseCam->GetViewTrans());

	// 
	m_pSkyBox = new SkyBox(pCube);
	m_pSkyBox->Create(SkyBox::SkyType::FROZEN);

	// no normals == no mesh #TODO: CalcNormals() if none
	m_pGroundMesh = new OCMesh(pGround, glm::vec4(0, -2, 0, 1));
	m_pGroundMesh->Load("_Resources/Objects/ground.obj", "_Resources/Objects/");

	m_pMonkeyMesh = new OCMesh(pMonkey);
	m_pMonkeyMesh->Load("_Resources/Objects/monkey.obj", "_Resources/Objects/");

}

void TestScene::ShutDown()
{
	BaseApp::ShutDown();
}

void TestScene::Update(const double a_dt)
{
	// Update GPU with Camera Updates:
	m_pCameraUniBuff->Update();

	double elap = m_pTime->GetElapsedTime();
	m_pMonkeyMesh->AddRotation( a_dt/50, glm::vec3(0, 1, 0) );
}

void TestScene::Render()
{
	// set up internal update (only update every x seconds)
	m_pShader->UpdatePrograms();

	m_pSkyBox->Render();

	m_pGroundMesh->Render();
	m_pMonkeyMesh->Render();
}