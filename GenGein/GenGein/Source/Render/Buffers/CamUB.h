#pragma once
#include <glm\glm.hpp>

class CamUB
{
public:
	CamUB();
	~CamUB();

	void SetUp(const glm::mat4& a_camWlrd, const glm::mat4& a_camProj, const glm::mat4& a_camView);
	void CleanUp();

	void Update();

private:

	// TODO: Improve data usage/storage
	// - Remove copy and update GPU mem with pointers
	struct POINTERS_TO_DATA
	{
		const glm::mat4* camWorld;
		const glm::mat4* camProj;
		const glm::mat4* camView;
	} m_dataPointers;

	struct UNI_BUFF_STORAGE
	{
		glm::mat4 camWorld;
		glm::mat4 camProj;
		glm::mat4 camView;
	} m_unibuffstorage;

	unsigned int m_UBO;
	unsigned int m_blockIndex;
};