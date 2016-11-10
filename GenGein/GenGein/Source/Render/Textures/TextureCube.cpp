#include <gl_core_4_4.h>
#include <stb\stb_image.h>

#include "Console\Console.h"
#include "TextureCube.h"

TextureCube::TextureCube()
	: m_name("\0"), m_GLTexture(-1), m_GLSlot(-1), m_UniformLocation(-1)
{}

TextureCube::TextureCube(const unsigned int* a_programID, const std::string a_name) : TextureCube()
{
	// #NOTE: program mem copy could corrupt texture in shader program
	m_name = a_name;
	m_UniformLocation = glGetUniformLocation(*a_programID, m_name.c_str());
}

TextureCube::~TextureCube()
{
	glDeleteTextures(1, &m_GLTexture);
}

void TextureCube::AddTexturesFromPath(std::string a_path, unsigned int a_gl_textureSlot)
{
	//#TODO: Everything here
}

void TextureCube::AddUniqueTextures(std::vector<std::string> a_texturePaths, unsigned int a_textureSlot)
{
	int width, height, format;
	unsigned char* data;

	m_GLSlot = a_textureSlot;

	glGenTextures(1, &m_GLTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLTexture);

	for (unsigned int i = 0; i < a_texturePaths.size(); i++)
	{
		width = -1, height = -1, format = -1;

		data = stbi_load(a_texturePaths[i].c_str(), &width, &height, &format, STBI_default);

		if (data == NULL)
		{
			printf("Unable to find texture: %s thus Default assigned.\n", a_texturePaths[i].c_str());
			const char* src = "Data/Objects/default.png";

			data = stbi_load(src, &width, &height, &format, STBI_default);
		}

		format == 1 ? format = GL_RED	:
		format == 2 ? format = GL_RG	:
		format == 3 ? format = GL_RGB	:
		format == 4 ? format = GL_RGBA	:
		format == NULL;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
			width, height, 0, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::Render()
{
	int loc = m_GLSlot - GL_TEXTURE0;

	glActiveTexture(m_GLSlot);
	glBindTexture(GL_TEXTURE_2D, m_GLTexture);
	glUniform1i(m_UniformLocation, loc);
}