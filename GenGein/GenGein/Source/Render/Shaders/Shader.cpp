#include <string>
#include <fstream>

#include "Core\GL\gl_core_4_4.h"
#include "Console\Console.h"

#include "Shader.h"

//#NOTE: Should probably remove
// 'ExtractFromFile' function and 
// make a 'File' class to handle that

Shader::Shader()
	: m_shaderType(),
	m_directory("\0"),
	m_shaderID(-1)
{}

Shader::~Shader()
{
	glDeleteBuffers(1, &m_shaderID);
}

bool Shader::Load(std::string a_shaderPath, ShaderType a_shaderType)
{
	m_directory = a_shaderPath;
	m_shaderType = a_shaderType;

	if (m_directory.empty())
		return false;

	std::string shader_src = ExtractFromFile(m_directory);

	if (shader_src.empty())
		return false;

	if (!CreateShader(shader_src, a_shaderType))
		return false;

	shader_src.clear();

	return true;
}

bool Shader::AttachToProgram(unsigned int a_program)
{
	if (m_shaderID != -1)
		Console::Log("#SUC | %s VALID SHADER, ATTACH SUCCESSFUL\n", m_directory.c_str());
	else
		return false;

	glAttachShader(a_program, m_shaderID);
	return true;
}

void Shader::CleanUp()
{
	if (m_shaderID != -1)
	{
		glDeleteShader(m_shaderID);
		m_shaderID = -1;
	}

	if (!m_directory.empty())
		m_directory.clear();
		
}

// Private Functions:
bool Shader::CheckShaderStatus(unsigned int a_shader)
{
	// Retrieve success of compiling from shader object
	int32_t success = GL_FALSE;
	glGetShaderiv(a_shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		// Retrieve the log size
		int32_t infoLogLength = 0;
		glGetShaderiv(a_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		// Print out the log
		glGetShaderInfoLog(a_shader, infoLogLength, 0, infoLog);
		Console::Log("#ERR | Failed to link shader!\n%s\n", infoLog);
		delete[] infoLog;
		return false;
	}
	return true;
}

bool Shader::CreateShader(std::string a_src, ShaderType a_shaderType)
{
	unsigned int sID = -1;

	if (a_src.empty())
		return false;

	sID = glCreateShader((unsigned int)a_shaderType);

	const char* src = a_src.c_str();
	glShaderSource(sID, 1, (const char**)&src, 0);
	glCompileShader(sID);

	if (!CheckShaderStatus(sID))
	{
		Console::Log("#ERR | SHADER DIRECTORY %s IS INVALID\n", m_directory.c_str());
		return false;
	}
	else
		Console::Log("#SUC | SHADER INITIALISED %s\n", m_directory.c_str());
	
	m_shaderID = sID;

	return true;
}

//#NOTE: MOVE INTO SEPERATE CLASS
std::string Shader::ExtractFromFile(std::string a_filePath)
{
	std::string content = "", line = "";
	std::ifstream src(a_filePath, std::ios::in);

	// Grab the shader source
	if (!src)
	{
		Console::Log("#ERR | COULD NOT READ FILE AT %s FILE NON-EXISTENT\n", a_filePath.c_str());
		return std::string();
	}
	
	while (std::getline(src, line))
	{	
		content.append(line + "\n");
	}

	src.close();
	return content;
}
