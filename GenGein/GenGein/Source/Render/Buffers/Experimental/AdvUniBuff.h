#pragma once
#include <vector>

/*
* HIGHLY experimental class.
* attempting to generalise uniform input
* to allow generic input
*/
class AdvUniBuff
{
public:
	AdvUniBuff();
	//AdvUniBuff(unsigned int* a_programID);
	~AdvUniBuff();

	void SetUp();
	virtual void CleanUp();

	virtual void Update();

protected:

	// Helper functions to convert incoming variables to floats
	// #TODO - remove these helper functions and use memcpy instead
	void AddVec4ToVector(std::vector<const float*>& a_out, const glm::vec4& a_in);
	void AddMat4ToVector(std::vector<const float*>& a_out, const glm::mat4& a_in);

	bool UpdateBuffer(std::vector<float>& a_stream, std::vector<const float*> a_refStream);
	
	// Whether to update the stream or not
	bool m_updateBuffer;

	// Information stream sent to the GPU
	size_t m_bufferSize;
	std::vector<float> m_bufferStream;

private:
	// TODO: Improve data usage/storage
	// - Remove copy and update GPU mem with pointers

	// GL specific uniform buffer variables
	//unsigned int* m_programID;
	unsigned int m_UBO;
	unsigned int m_blockIndex;
};