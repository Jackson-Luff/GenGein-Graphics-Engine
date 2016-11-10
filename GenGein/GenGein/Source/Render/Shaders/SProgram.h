#pragma once
//#NOTE/#TODO: REMOVE IN FUTURE
#include "Shader.h"

class SProgram
{
public:
	SProgram();
	SProgram(const char* a_name);
	~SProgram();

	// Attach individual Shader via directory and type
	bool AttachShader(const char*, Shader::ShaderType);
	// Attach shaders from a given directory and common name
	bool AttachShadersFromDir(const char*, const char*);
	// Once shaders are validated, link them to the program
	bool CompositeProgram();
	// Shutdown/cleanup any left behind bits
	void Shutdown();
	// Enable this program for rendering
	void UseProgram();

	// Get the name of the program
	const char* GetName();
	// Get the GL ID
	unsigned int GetID();
private:
	// Validate Program Status
	bool CheckProgramStatus(unsigned int a_prog);

	// Name of program
	const char* m_programName;
	// ID of the program
	unsigned int m_programID;
	// Shaders of program
	Shader* m_vertShader;
	Shader* m_fragShader;
	Shader* m_geomShader;
	Shader* m_tessCShader;
	Shader* m_tessEShader;
};
