#include "SProgram.h"
#include "NShaderHandle.h"

std::vector<SProgram*> NShaderHandle::m_programList = std::vector<SProgram*>();

NShaderHandle::NShaderHandle()
{
}

NShaderHandle::~NShaderHandle()
{
}

bool NShaderHandle::AddProgram(SProgram* a_program)
{
	if (a_program != nullptr)
	{
		m_programList.push_back(a_program);
		return true;
	}

	printf("ERR: CANNOT ADD. %s IS AN INVALID PROGRAM", a_program);
	return false;
}

bool NShaderHandle::RemoveProgram(const char* a_programName)
{
	for (unsigned int i = 0; i < m_programList.size(); i++)
	{
		if (m_programList[i] != nullptr)
		{
			if (m_programList[i]->GetName() == a_programName)
			{
				m_programList[i]->Shutdown();
				m_programList.erase(m_programList.begin() + i);
				return true;
			}
		}	
	}

	printf("ERR: UNABLE TO REMOVE PROGRAM | %s NOT FOUND", a_programName);
	return false;
}
SProgram* NShaderHandle::GetProgram(const char* a_programName)
{
	for (unsigned int i = 0; i < m_programList.size(); i++)
	{
		if (m_programList[i] != nullptr)
		{
			if (m_programList[i]->GetName() == a_programName)
				return m_programList[i];
		}
	}
	
	printf("ERR: NO PROGRAM INSTANCE OF %S FOUND", a_programName);
	return nullptr;
}