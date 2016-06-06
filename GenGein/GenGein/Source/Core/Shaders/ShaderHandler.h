#pragma once
#include <map>

////
// Author: Jackson Luff
// Name: ShaderLoader
// Type: class (standard)
// Parent: None
// Description:
// * ShaderLoader loads and handles shader 
// * formatted files and applies accordingly.
////
class ShaderHandler
{
public:
	// Type of shader loaded
	enum class ShaderType : unsigned int
	{
		VERT_SHADER = 0x8B31,	// GL_VERTEX_SHADER
		FRAG_SHADER = 0x8B30,	// GL_FRAGMENT_SHADER
		GEOM_SHADER = 0x8DD9,	// GL_GEOMETRY_SHADER
		TESSC_SHADER = 0x8E88,	// GL_TESS_CONTROL_SHADER
		TESSE_SHADER = 0x8E87,	// GL_TESS_EVALUATION_SHADER
	};

	struct Shader
	{
	public:
	private:
		ShaderType	ID;
		const char* Name;
		const char* directory;
	};

	// Store Directories for Hot-loading
	struct DirectoryData
	{
		const char* vertDir;
		const char* pixeDir;
		const char* geomDir;
		const char* tesCDir;
		const char* tesEDir;
	};
	
	~ShaderHandler();

	//Getter for the shader program
	static unsigned int GetShader(const char* a_shaderName);

	// Initialise Shader Content
	static const unsigned int CreateShader(const char* a_shaderDir, ShaderType a_type);

	// Create a GL program with inputted vert and frag shaders
	static const unsigned int ShaderHandler::LoadShaderProgram(
		const char* a_shaderName, const char* a_vertShader,
		const char* a_fragShader, const char* a_geomShader,
		const char* a_tessCShader, const char* a_tessEShader);

	// Reloads All programs (hot-loading)
	static void ReloadAllPrograms();

	// Unloads All Programs
	static void UnloadAllPrograms();
private:

	// Returns if the programID was found in the map
	static bool DoesShaderExist(const char* a_shaderName);
	// Determines the success of the shader
	static bool CheckShaderStatus(unsigned int a_shaderName);
	// Determines the success of the program
	static bool CheckProgramStatus(unsigned int prog);
	// Reads the shader based on the directory 
	static const char* ReadShaderCode(const char* a_filePath);

	static std::map< unsigned int, DirectoryData> m_directoryMap;
	static std::map< const char*, unsigned int > m_programMap;
};