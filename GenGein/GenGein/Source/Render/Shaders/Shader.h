#pragma once
#include <string>

class Shader
{
public:

	enum class ShaderType : unsigned int
	{
		VERT_SHADER = 0x8B31,  //GL_VERTEX_SHADER
		FRAG_SHADER = 0x8B30,  //GL_VERTEX_SHADER
		GEOM_SHADER = 0x8DD9,  //GL_GEOMETRY_SHADER,
		TESSC_SHADER = 0x8E88, //GL_TESS_CONTROL_SHADER,
		TESSE_SHADER = 0x8E87, //GL_TESS_EVALUATION_SHADER,
		DEFAULT = 0x000
	};

	Shader();
	~Shader();

	// Load up the shader from a given directory
	bool Load(std::string, ShaderType);
	// Attach the shader to the intended program
	bool AttachToProgram(unsigned int);
	// Cleans up after program attachement
	void CleanUp();
private:
	// Creates shader buffer
	bool CreateShader(std::string, ShaderType);
	// Validate shader status
	bool CheckShaderStatus(unsigned int a_shader);
	// Reads the shader file
	std::string ExtractFromFile(std::string);

	// Stores directory for hotloading
	std::string m_directory;
	// Shader ID
	unsigned int m_shaderID;
	// Determines type of shader this is
	ShaderType m_shaderType;
};