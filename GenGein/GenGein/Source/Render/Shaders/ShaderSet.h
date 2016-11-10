#pragma once
#include <glm\glm.hpp>

#include <vector>
#include <utility>
#include <map>
#include <set>

// credit: https://github.com/nlguillemot/ShaderSet/blob/master/shaderset.h

class ShaderSet
{
	// typedefs for readability
	using ShaderHandle = unsigned int;
	using ProgramHandle = unsigned int;

	// filename and shader type
	struct ShaderNameTypePair
	{
		std::string Name;
		unsigned int Type;
		bool operator<(const ShaderNameTypePair& rhs) const { return std::tie(Name, Type) < std::tie(rhs.Name, rhs.Type); }
	};

	// Shader in the ShaderSet system
	struct Shader
	{
		ShaderHandle Handle;
		// Timestamp of the last update of the shader
		glm::i64 Timestamp;
		// Hash of the name of the shader. This is used to recover the shader name from the GLSL compiler error messages.
		// It's not a perfect solution, but it's a miracle when it doesn't work.
		int HashName;
	};

	// Program in the ShaderSet system.
	struct Program
	{
		// The handle exposed externally ("public") and the most recent (succeeding/failed) linked program ("internal")
		// the public handle becomes 0 when a linking failure happens, until the linking error gets fixed.
		ProgramHandle PublicHandle;
		ProgramHandle InternalHandle;
	};

	// the version in the version string that gets prepended to each shader
	std::string mVersion;
	// the preamble which gets prepended to each shader (for eg. shared binding conventions)
	std::string mPreamble;
	// maps shader name/types to handles, in order to reuse shared shaders.
	std::map<ShaderNameTypePair, Shader> mShaders;
	// allows looking up the program that represents a linked set of shaders
	std::map<std::vector<const ShaderNameTypePair*>, Program> mPrograms;

public:
	ShaderSet() = default;

	// Destructor releases all owned shaders
	~ShaderSet();

	// The version string to prepend to all shaders
	// Separated from the preamble because #version doesn't compile in C++
	void SetVersion(const std::string& version);

	// A string that gets prepended to every shader that gets compiled
	// Useful for compile-time constant #defines (like attrib locations)
	void SetPreamble(const std::string& preamble);

	// Convenience for reading the preamble from a file
	// The preamble is NOT auto-reloaded.
	void SetPreambleFile(const std::string& preambleFilename);

	// list of (file name, shader type) pairs
	// eg: AddProgram({ {"foo.vert", GL_VERTEX_SHADER}, {"bar.frag", GL_FRAGMENT_SHADER} });
	unsigned int* AddProgram(const std::vector<std::pair<std::string, unsigned int>>& typedShaders);

	// Polls the timestamps of all the shaders and recompiles/relinks them if they changed
	void UpdatePrograms();

	// Convenience to add shaders based on extension file naming conventions
	// vertex shader: .vert
	// fragment shader: .frag
	// geometry shader: .geom
	// tessellation control shader: .tesc
	// tessellation evaluation shader: .tese
	// compute shader: .comp
	// eg: AddProgramFromExts({"foo.vert", "bar.frag"});
	unsigned int* AddProgramFromExts(const std::vector<std::string>& shaders);
};