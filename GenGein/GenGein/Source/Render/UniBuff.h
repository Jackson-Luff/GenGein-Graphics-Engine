#pragma once
#include <glm\fwd.hpp>

class UniBuff
{
public:
	UniBuff(unsigned int);
	~UniBuff();

	void SetUp(const glm::vec4&, const glm::mat4&);
	void CleanUp();

	void Update();

private:
	
	// TODO: Improve data usage/storage
	// - Remove copy and update GPU mem with pointers
	struct POINTERS_TO_DATA
	{
		const glm::vec4* camPosition;
		const glm::mat4* camProjView;
	} m_dataPointers;

	struct UNI_BUFF_STORAGE
	{
		glm::vec4 camPosition;
		glm::mat4 camProjView;
	} m_unibuffstorage;

	unsigned int m_programID;
	unsigned int m_UBO;
	unsigned int m_blockIndex;

};