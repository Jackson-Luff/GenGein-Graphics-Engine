#include <string>

#include "Core\GL\gl_core_4_4.h"
#include "Console\Console.h"

#include "SProgram.h"

using ShaderID = Shader::ShaderType;

SProgram::SProgram() :
	m_programName("\0"), m_programID(0),
	m_vertShader(nullptr), m_fragShader(nullptr),
	m_geomShader(nullptr), m_tessCShader(nullptr),
	m_tessEShader(nullptr)
{}

SProgram::SProgram(const char* a_name)
	: SProgram()
{
	m_programName = a_name;
}

SProgram::~SProgram()
{}

bool SProgram::AttachShader(const char* a_directory, ShaderID a_type)
{
	Shader* shader = new Shader();
	
	if (!shader->Load(a_directory, a_type))
		return false;

	shader->AttachToProgram(m_programID);
	switch (a_type)
	{
	case ShaderID::VERT_SHADER:
		m_vertShader = shader;
		break;
	case ShaderID::FRAG_SHADER:
		m_fragShader = shader;
		break;
	case ShaderID::GEOM_SHADER:
		m_geomShader = shader;
		break;
	case ShaderID::TESSC_SHADER:
		m_tessCShader = shader;
		break;
	case ShaderID::TESSE_SHADER:
		m_tessEShader = shader;
		break;
	case ShaderID::DEFAULT:
		return false;
	default:
		return false;
	}

	return true;
}

Shader& GenShader(std::string a_dir, ShaderID a_ID)
{
	Shader* shader = new Shader();
	shader->Load(a_dir, a_ID);

	return *shader;
}

bool SProgram::AttachShadersFromDir(const char* a_folderDir, const char* a_comName)
{
	std::string dir = std::string(a_folderDir).append(a_comName);
	
	// #NOTE -> DID HAVE CODE IN A FOR LOOP BUT PROCESS BECAME INEFFICIENT/DIRTY
	m_vertShader = &GenShader(std::string(dir + ".vert"), ShaderID::VERT_SHADER);
	m_fragShader = &GenShader(std::string(dir + ".frag"), ShaderID::FRAG_SHADER);

	return true;
}

bool SProgram::CompositeProgram()
{
	m_programID = glCreateProgram();

	m_vertShader->AttachToProgram(m_programID);
	m_fragShader->AttachToProgram(m_programID);
	//m_geomShader->AttachToProgram(m_programID);
	//m_tessCShader->AttachToProgram(m_programID);
	//m_tessEShader->AttachToProgram(m_programID);

	glLinkProgram(m_programID);

	if (!CheckProgramStatus(m_programID))
		return false;

	m_vertShader->CleanUp();
	m_fragShader->CleanUp();
	//m_geomShader->CleanUp();
	//m_tessCShader->CleanUp();
	//m_tessEShader->CleanUp();

	Console::Log("#SUC | %s Program Initialised\n", m_programName);
	return true;
}

void SProgram::Shutdown()
{
	if (m_programID != -1)
		glDeleteProgram(m_programID);

	glUseProgram(0);
}

void SProgram::UseProgram()
{
	glUseProgram(m_programID);
}

const char* SProgram::GetName()
{
	return m_programName;
}

unsigned int SProgram::GetID()
{
	return m_programID;
}

// Private Func's:
bool SProgram::CheckProgramStatus(unsigned int a_prog)
{
	// Retrieve success of linking from program object
	int32_t success = GL_FALSE;
	glGetProgramiv(a_prog, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		// Retrieve the log size
		int32_t infoLogLength = 0;
		glGetProgramiv(a_prog, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		// Print out the log
		glGetProgramInfoLog(a_prog, infoLogLength, 0, infoLog);
		Console::Log("#ERR | Failed to load program!\n%s\n", infoLog);
		delete[] infoLog;
		return false;
	}

	// Program is successful!
	return true;
}