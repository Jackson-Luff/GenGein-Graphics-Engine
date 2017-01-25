#pragma once

class Shape;

class GBuffer
{
public:

	enum GBUFFER_TEXTURE_TYPE : unsigned int
	{
		GTEXTURE_POSITION,
		GTEXTURE_NORMAL,
		GTEXTURE_ALBEDO,
		GTEXTURE_COMP,
		GTEXTURES_NUM
	};

	//
	GBuffer();
	GBuffer(const unsigned int*);
	~GBuffer();

	//
	bool SetUp(const int, const int);
	//
	void Render();

	//
	void BindForWriting();
	void BindForReading();

	//protected, for now.
protected:

	//
	Shape* m_postQuad;
	//
	const unsigned int* m_pGBuffProgID;
	//
	unsigned int m_gBufferID;
	//
	unsigned int m_gTextures[GBUFFER_TEXTURE_TYPE::GTEXTURES_NUM];
	//
	unsigned int m_depthTexture;
};