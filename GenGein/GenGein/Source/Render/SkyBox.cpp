#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <string>
#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"
#include "Skybox.h"

SkyBox::SkyBox()
{}

SkyBox::~SkyBox()
{
	glDeleteBuffers(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void SkyBox::Create(const SkyType& a_presetType)
{
	InitialiseDirs(a_presetType);

	LoadShaderProgram();

	LoadCubeTextures(m_presetDirectires);

	LoadCubeVertices();

	printf("SUCCESS: SkyBox Load Successful.\n\n");
}

void SkyBox::Render(const glm::mat4& a_proj, const glm::mat4& a_view)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	glUseProgram(m_skyShaderID);
	glBindVertexArray(m_vao);

	int loc = glGetUniformLocation(m_skyShaderID, "Projection");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &a_proj[0][0]);

	loc = glGetUniformLocation(m_skyShaderID, "View");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &a_view[0][0]);

	glUniform1i(m_cubemapUniLoc, m_cubeMapID);
	glActiveTexture(m_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

// Private Functions:

void SkyBox::InitialiseDirs(const SkyBox::SkyType& a_presetType)
{
	m_presetDirectires = std::vector<std::string>(6);

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
		skyDir = "Data/SkyBox/Sky/";
		break;
	case SkyBox::SkyType::SPACE:
		skyDir = "Data/SkyBox/Space/";
		break;
	case SkyBox::SkyType::CHAPEL:
		skyDir = "Data/SkyBox/Chapel/";
		break;
	default:
		printf("ERROR: No such directory (skybox).\n");
		return;
		break;
	}

	for (int i = 0; i < 6; i++)
		m_presetDirectires[i] = skyDir + names[i] + fileType;
}

const char* ReadShaderCode(const char* a_filePath)
{
	if (!a_filePath)
		return NULL;

	// Grab the shader source
	FILE* file = fopen(a_filePath, "rb");
	if (!file)
		return NULL;

	// If it's a bad file, get outta there.
	if (fseek(file, 0, SEEK_END) == -1)
		return NULL;

	unsigned int length = ftell(file);

	if (length == -1)
		return NULL;

	if (fseek(file, 0, SEEK_SET) == -1)
		return NULL;

	// Populate return char string
	char* content = new char[length + 1];
	memset(content, 0, length + 1);

	if (content == NULL)
		return NULL;

	fread(content, sizeof(char), length, file);

	// Check for errors
	if (ferror(file))
	{
		free(content);
		return NULL;
	}

	// Delete pointer data
	fclose(file);
	content[length] = '\0';

	// Successful load!
	return content;
}

void CreateShader(const char* a_dir, const unsigned int a_SHADER, unsigned int& a_ID)
{
	const char* src = ReadShaderCode(a_dir);

	if (a_dir == NULL || src == NULL)
		return;

	a_ID = glCreateShader(a_SHADER);

	glShaderSource(a_ID, 1, (const char**)&src, 0);
	glCompileShader(a_ID);

	printf("SUCCESS: Shader initialised: %s\n", a_dir);
}

void SkyBox::LoadShaderProgram()
{
	unsigned int vertShader;
	unsigned int fragShader;
	CreateShader("Data/SkyBox/Sky.vert", GL_VERTEX_SHADER, vertShader);
	CreateShader("Data/SkyBox/Sky.frag", GL_FRAGMENT_SHADER, fragShader);

	// Apply shaders to program
	m_skyShaderID = glCreateProgram();
	glAttachShader(m_skyShaderID, vertShader);
	glAttachShader(m_skyShaderID, fragShader);
	glLinkProgram(m_skyShaderID);

	// Check success of program for stable use
	//if (!CheckProgramStatus(progID))
	//	return -1;

	// Deleting shaders/program to
	// minimize memory leaks
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	printf("SUCCESS: Custom Program initialised\n\n");
}

void SkyBox::LoadCubeVertices()
{
	float points[] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,

		+1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,

		+1.0f, -1.0f, -1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,

		+1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		-1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 108, NULL);

	glBindVertexArray(0);
}

void SkyBox::LoadCubeTextures(const std::vector<std::string>& a_dirs)
{
	glGenTextures(1, &m_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);

	int width, height, format;
	unsigned char* data;

	for (unsigned int i = 0; i < a_dirs.size(); i++)
	{
		width = 0, height = 0, format = 0;

		data = stbi_load(a_dirs[i].c_str(), &width, &height, &format, STBI_default);

		if (data == NULL)
		{
			printf("Unable to find texture: %s thus Default assigned.\n", a_dirs[i].c_str());
			const char* src = "Data/Objects/default.png";

			data = stbi_load(src, &width, &height, &format, STBI_default);
		}

		if (format == 1)
			format = GL_RED;
		if (format == 2)
			format = GL_RG;
		if (format == 3)
			format = GL_RGB;
		if (format == 4)
			format = GL_RGBA;

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

	m_cubemapUniLoc = glGetUniformLocation(m_skyShaderID, "Skybox");
}