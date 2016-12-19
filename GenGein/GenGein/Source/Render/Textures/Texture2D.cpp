#include <gl_core_4_4.h>
#include <stb\stb_image.h>

#include "Input\Console\Console.h"
#include "Texture2D.h"

using C_LOG_TYPE = Console::LOG_TYPE;

Texture2D::Texture2D() :
	m_name(std::string()), 
	m_GLTexture(-1), m_GLSlot(-1), m_UniformLocation(-1)
{}

Texture2D::Texture2D(unsigned int* a_programID, std::string a_name) : Texture2D()
{
	// #NOTE: program mem copy could corrupt texture in shader program
	m_name = a_name;
	m_UniformLocation = glGetUniformLocation(*a_programID, m_name.c_str());
}

Texture2D::~Texture2D()
{
}

void Texture2D::Setup(std::string a_directory, unsigned int a_gl_textureSlot)
{
	m_GLSlot = a_gl_textureSlot;

	int imgWidth = 0, imgHeight = 0, imgFormat = 0;
	const char* path = a_directory.c_str();

	unsigned char* data = stbi_load(path, &imgWidth, &imgHeight, &imgFormat, STBI_default);

	if (data == NULL)
		return Console::Log(C_LOG_TYPE::LOG_WARNING, "Missing texture at: %s\n", path);

	imgFormat == 1 ? imgFormat = GL_RED  :
	imgFormat == 2 ? imgFormat = GL_RG   :
	imgFormat == 3 ? imgFormat = GL_RGB  :
	imgFormat == 4 ? imgFormat = GL_RGBA :
	imgFormat == NULL;

	glGenTextures(1, &m_GLTexture);
	glBindTexture(GL_TEXTURE_2D, m_GLTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, imgWidth, imgHeight, 0, imgFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	Console::Log(C_LOG_TYPE::LOG_SUCCESS, "%s Texture Loaded.\n", m_name.c_str());
}

void Texture2D::Shutdown()
{
	glDeleteTextures(1, &m_GLTexture);
}

void Texture2D::Render()
{
	int loc = m_GLSlot - GL_TEXTURE0;

	glActiveTexture(m_GLSlot);
	glBindTexture(GL_TEXTURE_2D, m_GLTexture);
	glUniform1i(m_UniformLocation, loc);
}