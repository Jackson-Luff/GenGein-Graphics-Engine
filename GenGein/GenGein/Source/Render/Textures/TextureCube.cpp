#include <gl_core_4_4.h>
#include <stb\stb_image.h>

#include "Input\Console\Console.h"
#include "TextureCube.h"

TextureCube::TextureCube()
	: m_name("\n"), m_GLTexture(-1), m_GLSlot(-1), m_UniformLocation(-1)
{}

TextureCube::TextureCube(const unsigned int* a_programID, const std::string a_name) : TextureCube()
{
	// #NOTE: program mem copy could corrupt texture in shader program
	m_name = a_name;
	m_UniformLocation = glGetUniformLocation(*a_programID, a_name.c_str());
}

TextureCube::~TextureCube()
{
	glDeleteTextures(1, &m_GLTexture);
}

void TextureCube::AddTexturesFromPath(std::string a_path, unsigned int a_gl_textureSlot)
{
	//#TODO: Everything here
}

void TextureCube::AddUniqueTextures(std::vector<std::string> a_skyPath, unsigned int a_textureSlot)
{
	int width, height, format;
	unsigned char* data;

	m_GLSlot = a_textureSlot;

	glGenTextures(1, &m_GLTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLTexture);

	for(size_t i = 0; i < a_skyPath.size(); ++i)
	{
		width = -1, height = -1, format = -1;
		const char* path = a_skyPath[i].c_str();

		data = stbi_load(path, &width, &height, &format, STBI_default);

		if (data == NULL)
		{
			Console::Log(Console::FBACK::LOG_WARNING, "Missing texture at: %s\n", path);
			continue;
		}

		format == 1 ? format = GL_RED	:
		format == 2 ? format = GL_RG	:
		format == 3 ? format = GL_RGB	:
		format == 4 ? format = GL_RGBA	:
		format == NULL;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

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
	//#NOTE: Texture Render DOES NOT need to be called every-frame
	int loc = m_GLSlot - GL_TEXTURE0;

	glActiveTexture(m_GLSlot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLTexture);
	glUniform1i(m_UniformLocation, loc);
}