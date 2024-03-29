#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <string>
#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "Input\Console\Console.h"
#include "Render\Shaders\ShaderSet.h"
#include "Render\Objects\Components\Shape.h"
#include "Render\Textures\TextureCube.h"

#include "Skybox.h"

using SkyTheme = SkyBox::SkyTheme;

std::vector<std::string> GetSmartDirectories(const SkyTheme a_presetType)
{
	std::string skyDir = "_Resources/Textures/SkyBoxes";
	std::string fileType = ".tga";
	std::string names[6]
	{
		"posz", "negz",
		"posy", "negy",
		"posx", "negx",
	};

	// #TODO: This is gross as fuck. Handle this better. <3 ~ yourself
	//Setup Sea Directory
	switch (a_presetType)
	{
	case SkyTheme::SKY:				skyDir.append("/Sky/");				 fileType = ".jpg"; break;
	case SkyTheme::SPACE:			skyDir.append("/Space/");			 fileType = ".jpg"; break;
	case SkyTheme::CHAPEL:			skyDir.append("/Chapel/");			 fileType = ".jpg"; break;
	case SkyTheme::GOLDRUSH:			skyDir.append("/Goldrush/");		 fileType = ".tga"; break;
	case SkyTheme::FROZEN:			skyDir.append("/Frozen/");			 fileType = ".tga"; break;
	case SkyTheme::MORNING:			skyDir.append("/Morning/");			 fileType = ".tga"; break;
	case SkyTheme::SUNSET:			skyDir.append("/SunSet/");			 fileType = ".png";	break;
	case SkyTheme::DARKSTORMY:		skyDir.append("/DarkStormy/");		 fileType = ".png"; break;
	case SkyTheme::TROPICALSUNNY:	skyDir.append("/TropicalSunny/");	 fileType = ".png"; break;
	case SkyTheme::CLOUDYLIGHTRAYS:	skyDir.append("/CloudyLightRays/");  fileType = ".png"; break;
	case SkyTheme::THICKCLOUDSWATER:	skyDir.append("/ThickCloudsWater/"); fileType = ".png"; break;
	default:
		Console::Log(Console::FBACK::LOG_ERROR, "No such directory %s.\n", skyDir.c_str());
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
	glDeleteProgram(*m_pSkyShader);
	delete m_pShape;
	delete m_pTextureCube;
}

void SkyBox::Create(const SkyTheme a_presetType)
{
	std::vector<std::string> directories = GetSmartDirectories(a_presetType);

	m_pShape = new Shape();
	m_pShape->Create(Shape::Geometry::CUBE);

	m_pTextureCube = new TextureCube(m_pSkyShader, "SkyBox");
	m_pTextureCube->AddUniqueTextures(directories, GL_TEXTURE0);

	Console::Log(Console::FBACK::LOG_SUCCESS, "Skybox Loaded Successfully. \n");
}

void SkyBox::Render()
{
	glDepthMask(GL_FALSE);
	
	glUseProgram(*m_pSkyShader);
	m_pTextureCube->Render();
	m_pShape->Render();

	glDepthMask(GL_TRUE);
}
