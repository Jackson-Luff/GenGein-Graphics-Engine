#include <fstream>
#include <glm\glm.hpp>

#include "Core\GL\gl_core_4_4.h"

#include "ShaderHandler.h"

using ShaderMap		= std::map<const char*, unsigned int>;
using DirectoryMap	= std::map<unsigned int, ShaderHandler::DirectoryData>;
using ShaderType	= ShaderHandler::ShaderType;

ShaderMap ShaderHandler::m_programMap = ShaderMap();
DirectoryMap ShaderHandler::m_directoryMap = DirectoryMap();

// Deconstructor
ShaderHandler::~ShaderHandler()
{
}

//Returns shader based on naming
unsigned int ShaderHandler::GetShader(const char* a_shaderName)
{
	if (DoesShaderExist(a_shaderName))
		return m_programMap[a_shaderName];

	return -1;
	printf("ERROR: No shader exists for that name!\n");
}

//Detects valid shader ID 
void ShaderValidationAndApplication(const char* a_name, unsigned int a_progID, unsigned int a_shaderID)
{
	if (a_shaderID != -1)
		glAttachShader(a_progID, a_shaderID);
	else
		printf("WARNING: Invalid shader entry for program: %s.\n", a_name);
}

// Create the program with shader file directories
const unsigned int ShaderHandler::LoadShaderProgram(
	const char* a_shaderName, const char* a_vertShader, 
	const char* a_fragShader, const char* a_geomShader, 
	const char* a_tessCShader, const char* a_tessEShader)
{
	if (DoesShaderExist(a_shaderName))
		return m_programMap[a_shaderName];

	unsigned int vertShader  = CreateShader(a_vertShader, ShaderType::VERT_SHADER);
	unsigned int fragShader  = CreateShader(a_fragShader, ShaderType::FRAG_SHADER);
	unsigned int geoShader	  = CreateShader(a_geomShader, ShaderType::GEOM_SHADER);
	unsigned int tessCShader = CreateShader(a_tessCShader, ShaderType::TESSC_SHADER);
	unsigned int tessEShader = CreateShader(a_tessEShader, ShaderType::TESSE_SHADER);

	// Apply shaders to program
	unsigned int progID = glCreateProgram();

	ShaderValidationAndApplication(a_shaderName, progID, vertShader);
	ShaderValidationAndApplication(a_shaderName, progID, fragShader);
	ShaderValidationAndApplication(a_shaderName, progID, geoShader);
	ShaderValidationAndApplication(a_shaderName, progID, tessCShader);
	ShaderValidationAndApplication(a_shaderName, progID, tessEShader);

	glLinkProgram(progID);

	// Check success of program for stable use
	if (!CheckProgramStatus(progID))
		return -1;

	// Deleting shaders/program to
	// minimize memory leaks

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(geoShader);
	//glDeleteShader(tessCShader);
	//glDeleteShader(tessEShader);

	printf("SUCCESS: Custom Program initialised\n\n");

	m_programMap[a_shaderName] = progID;

	DirectoryData dataDir;
	dataDir.vertDir = a_vertShader;
	dataDir.pixeDir = a_fragShader;
	dataDir.geomDir = a_geomShader;
	dataDir.tesCDir = a_tessCShader;
	dataDir.tesEDir = a_tessEShader;
	m_directoryMap[progID] = dataDir;

	return m_programMap[a_shaderName];
}

// Initialises the content via valid shader directory
const unsigned int ShaderHandler::CreateShader(const char* a_shaderDir, ShaderType a_type)
{
	unsigned int ID;
	const char* src = ReadShaderCode(a_shaderDir);

	if (a_shaderDir == NULL || src == NULL)
		return -1;

	ID = glCreateShader((unsigned int)a_type);

	glShaderSource(ID, 1, (const char**)&src, 0);
	glCompileShader(ID);

	if (!CheckShaderStatus(ID))
	{
		printf("ERROR: Shader source directory invalid.\n");
		return -1;
	}

	printf("SUCCESS: Shader initialised: %s\n", a_shaderDir);
	return ID;
}

void ShaderHandler::ReloadAllPrograms()
{
	for (auto& it : m_programMap)
	{
		const char* name = it.first;
		DirectoryData& dirData = m_directoryMap[it.second];
		glDeleteProgram(it.second);

		LoadShaderProgram(name,
			dirData.vertDir,
			dirData.pixeDir,
			dirData.geomDir,
			dirData.tesCDir,
			dirData.tesEDir);
	}
}

void ShaderHandler::UnloadAllPrograms()
{
	for (auto& prog : m_programMap)
	{
		glDeleteProgram(prog.second);
	}

	m_programMap.clear();

	glUseProgram(0);
}

// Private Functions:

// Loops through the map to define if a shader already exists
bool ShaderHandler::DoesShaderExist(const char* a_fileName)
{
	if (m_programMap.find(a_fileName) == m_programMap.end())
		return false;

	return true;
}

// Checks the success of the link status within the program
bool ShaderHandler::CheckProgramStatus(unsigned int a_prog)
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
		printf("Error: Failed to load program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
		return false;
	}

	// Program is successful!
	return true;
}

// Checks the success of the compile status within the shader
bool ShaderHandler::CheckShaderStatus(unsigned int a_shader)
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
		printf("Error: Failed to link shader program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
		return false;
	}
	return true;
}

// Readers in the shader from a file source
const char* ShaderHandler::ReadShaderCode(const char* a_filePath)
{
	if (!a_filePath)
		return NULL;

	// Grab the shader source
	FILE* file = fopen(a_filePath, "rb");
	if (file == NULL)
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