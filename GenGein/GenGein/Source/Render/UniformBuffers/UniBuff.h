#pragma once
#include <glm\glm.hpp>

class UniBuff
{
public:
	UniBuff();
	~UniBuff();

	void SetUp(const glm::vec4& a_camPos, const glm::mat4& a_camProj, const glm::mat4& a_camView);
	void CleanUp();

	void Update();

private:

	// TODO: Improve data usage/storage
	// - Remove copy and update GPU mem with pointers
	struct POINTERS_TO_DATA
	{
		const glm::vec4* camPosition;
		const glm::mat4* camProj;
		const glm::mat4* camView;
	} m_dataPointers;

	struct UNI_BUFF_STORAGE
	{
		glm::vec4 camPosition;
		glm::mat4 camProj;
		glm::mat4 camView;
	} m_unibuffstorage;

	unsigned int m_UBO;
	unsigned int m_blockIndex;
};