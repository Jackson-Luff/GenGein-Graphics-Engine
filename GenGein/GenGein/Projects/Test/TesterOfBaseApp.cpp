#include <glm\glm.hpp>
#include "Core\Shaders\SProgram.h"
#include "Core\Time.h"	
#include "Console\Console.h"
#include "Input\Cameras\FlyCam.h"
#include "Render\Objects\GenMesh.h"
#include "Render\UniBuff.h"

#include "TesterOfBaseApp.h"

TesterOfBaseApp::TesterOfBaseApp()
{
	Console::Log("#WAR | Remember to remove TesterOfBaseApp\n");
}

TesterOfBaseApp::~TesterOfBaseApp()
{}

/*
void TesterOfBaseApp::CreateIcosahedron()
{
	const int Faces[] = {
		2, 1, 0,
		3, 2, 0,
		4, 3, 0,
		5, 4, 0,
		1, 5, 0,
		11, 6, 7,
		11, 7, 8,
		11, 8, 9,
		11, 9, 10,
		11, 10, 6,
		1, 2, 6,
		2, 3, 7,
		3, 4, 8,
		4, 5, 9,
		5, 1, 10,
		2, 7, 6,
		3, 8, 7,
		4, 9, 8,
		5, 10, 9,
		1, 6, 10 };

	const glm::vec3 vertices[] = {
		{ glm::vec3(0.000f, 0.000f, 1.000f) },
		{ glm::vec3(0.894f, 0.000f, 0.447f) },
		{ glm::vec3(0.276f, 0.851f, 0.447f) },
		{ glm::vec3(-0.724f, 0.526f, 0.447f) },
		{ glm::vec3(-0.724f, -0.526f, 0.447f) },
		{ glm::vec3(0.276f, -0.851f, 0.447f) },
		{ glm::vec3(0.724f, 0.526f, -0.447f) },
		{ glm::vec3(-0.276f, 0.851f, -0.447f) },
		{ glm::vec3(-0.894f, 0.000f, -0.447f) },
		{ glm::vec3(-0.276f, -0.851f, -0.447f) },
		{ glm::vec3(0.724f, -0.526f, -0.447f) },
		{ glm::vec3(0.000f, 0.000f, -1.000f) }
	};

	m_indexCount = sizeof(Faces) / sizeof(Faces[0]);

	// Create the VAO:
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create the VBO for positions:
	GLuint vboPos;
	glGenBuffers(1, &vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// Create the VBO for indices:
	GLuint indices;
	glGenBuffers(1, &indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Faces), Faces, GL_STATIC_DRAW);
}
*/

void TesterOfBaseApp::StartUp(const int a_width, const int a_height, const char* a_title, bool a_fullscreen)
{
	BaseApp::StartUp(a_width, a_height, a_title, a_fullscreen);

	m_pTestProgram = new SProgram("TESTIE");
	m_pTestProgram->AttachShadersFromDir("Data/Shaders/Test/", "Test");
	m_pTestProgram->CompositeProgram();

	//Initialise camera
	FlyCam* pFlyCam = new FlyCam(glm::vec3(10), glm::vec3(0), 2, 0.5f);
	m_pBaseCam = pFlyCam;

	m_pUniBuff = new UniBuff(m_pTestProgram->GetID());
	m_pUniBuff->SetUp(m_pBaseCam->GetPosition(), m_pBaseCam->GetProjViewTrans());

	m_pGenMesh = new GenMesh();
	m_pGenMesh->GenSegmentedGrid(100, 3.0f, false);
}

void TesterOfBaseApp::ShutDown()
{
	BaseApp::ShutDown();
}

void TesterOfBaseApp::Update(const double a_dt)
{
	m_pUniBuff->Update();
}

void TesterOfBaseApp::Render()
{
	m_pTestProgram->UseProgram();
	
	m_pGenMesh->Render();

}	