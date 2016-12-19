#pragma once

#include <string>


// #TODO: Convert into a multi-texture class
class Texture2D
{
public:

#pragma region Slots
	enum class GLSlot : unsigned int
	{
		TEXTURE0 = 0x84C0,
		TEXTURE1 = 0x84C1,
		TEXTURE10 = 0x84CA,
		TEXTURE11 = 0x84CB,
		TEXTURE12 = 0x84CC,
		TEXTURE13 = 0x84CD,
		TEXTURE14 = 0x84CE,
		TEXTURE15 = 0x84CF,
		TEXTURE16 = 0x84D0,
		TEXTURE17 = 0x84D1,
		TEXTURE18 = 0x84D2,
		TEXTURE19 = 0x84D3,
		TEXTURE2 = 0x84C2,
		TEXTURE20 = 0x84D4,
		TEXTURE21 = 0x84D5,
		TEXTURE22 = 0x84D6,
		TEXTURE23 = 0x84D7,
		TEXTURE24 = 0x84D8,
		TEXTURE25 = 0x84D9,
		TEXTURE26 = 0x84DA,
		TEXTURE27 = 0x84DB,
		TEXTURE28 = 0x84DC,
		TEXTURE29 = 0x84DD,
		TEXTURE3 = 0x84C3,
		TEXTURE30 = 0x84DE,
		TEXTURE31 = 0x84DF,
		TEXTURE4 = 0x84C4,
		TEXTURE5 = 0x84C5,
		TEXTURE6 = 0x84C6,
		TEXTURE7 = 0x84C7,
		TEXTURE8 = 0x84C8,
		TEXTURE9 = 0x84C9,
	};
#pragma endregion

	Texture2D();
	Texture2D(unsigned int* a_program, std::string a_name);
	~Texture2D();

	void Setup(std::string a_directory, unsigned int a_gl_textureSlot);
	void Shutdown();

	void Render();
private:
	//#TODO: Change names
	std::string m_name;
	unsigned int m_GLTexture;
	unsigned int m_GLSlot;
	unsigned int m_UniformLocation;
};