#pragma once

// #TODO: Generic FBO class 
class FBuffer
{
public:

	FBuffer(const unsigned int a_frameAttachment);
	~FBuffer();

private:
	unsigned int m_bufferID;
};