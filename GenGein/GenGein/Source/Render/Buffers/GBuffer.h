#pragma once

class Shape;

class GBuffer
{
public:

	enum GBUFFER_TEXTURE_TYPE 
	{
		GTEXTURE_POSITION,
		GTEXTURE_NORMAL,
		GTEXTURE_ALBEDO,
		GTEXTURES_NUM
	};

	//
	GBuffer(unsigned int* a_programID);
	~GBuffer();

	//
	bool SetUp(const int a_wWidth, const int a_wHeight);
	void Render();

	//
	void BindForWriting();
	void BindForReading();
private:

	Shape* m_postQuad;

	//
	unsigned int* m_programID;
	//
	unsigned int m_gBufferID;
	//
	unsigned int m_gTextures[GTEXTURES_NUM];
	//
	unsigned int m_depthTexture;

	// Stored window dimensions
	int m_wWidth, m_wHeight;
};