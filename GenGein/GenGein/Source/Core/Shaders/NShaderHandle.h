#pragma once
#include <vector>

class SProgram;

class NShaderHandle
{
public:
	NShaderHandle();
	~NShaderHandle();

	static bool AddProgram(SProgram*);
	static bool RemoveProgram(const char*);
	static SProgram* GetProgram(const char*);

private:
	static std::vector<SProgram*> m_programList;
};

