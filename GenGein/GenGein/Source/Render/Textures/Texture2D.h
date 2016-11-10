#pragma once

#include <string>

// #TODO: Convert into a multi-texture class
class Texture2D
{
public:
	Texture2D();
	Texture2D(unsigned int a_program, std::string a_name);
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