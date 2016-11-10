#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <string>
#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "Console\Console.h"
#include "Render\Shaders\ShaderSet.h"
#include "Render\Objects\Components\Shape.h"
#include "Render\Textures\TextureCube.h"

#include "Skybox.h"

std::vector<std::string> GetSmartDirectories(const SkyBox::SkyType a_presetType)
{
	std::string skyDir;
	std::string fileType = ".jpg";
	std::string names[6]
	{
		"posx", "negx",
		"posy", "negy",
		"posz", "negz",
	};

	//Setup Sea Directory
	switch (a_presetType)
	{
		break;
	case SkyBox::SkyType::SKY:
		skyDir = "Data/Textures/SkyBoxes/Sky/";
		break;
	case SkyBox::SkyType::SPACE:
		skyDir = "Data/Textures/SkyBoxes/Space/";
		break;
	case SkyBox::SkyType::CHAPEL:
		skyDir = "Data/Textures/SkyBoxes/Chapel/";
		break;
	default:
		Console::Log("#ERR | No such directory %s.\n", skyDir.c_str());
		break;
	}

	std::vector<std::string> directories = std::vector<std::string>(6);

	for (int i = 0; i < 6; i++)
		directories[i] = skyDir + names[i] + fileType;

	return directories;
}

SkyBox::SkyBox()
{}

SkyBox::SkyBox(const unsigned int* a_program)
{
	m_pSkyShader = a_program;
}

SkyBox::~SkyBox()
{
	delete m_pShape;
	delete m_pTextureCube;
}

void SkyBox::Create(const SkyType a_presetType)
{
	std::vector<std::string> directories = GetSmartDirectories(a_presetType);

	m_pShape = new Shape();
	m_pShape->Create(Shape::CUBE);

	m_pTextureCube = new TextureCube(m_pSkyShader, "SkyBox");
	m_pTextureCube->AddUniqueTextures(directories, GL_TEXTURE0);

	Console::Log("#SUCC | Skybox Loaded Successfully. \n");
}

void SkyBox::Render()
{
	glDepthMask(GL_FALSE);
	
	m_pTextureCube->Render();
	m_pShape->Render();

	glDepthMask(GL_TRUE);
}
