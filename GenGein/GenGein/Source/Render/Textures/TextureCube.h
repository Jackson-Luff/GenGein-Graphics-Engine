#pragma once

class Texture2D;

#include <string>
#include <vector>

// #TODO: Convert into a multi-texture class
class TextureCube
{
public:
	TextureCube();
	TextureCube(const unsigned int* a_program, const std::string a_name);
	~TextureCube();

	// Convenience to add textures based on assumed texture naming convention
	// e.g. name of image: front, back, left, right, top, bottom
	void AddTexturesFromPath(const std::string a_path, const unsigned int a_gl_textureSlot);

	// Convenience to add textures based on name of image file naming conventions
	// NOTE: .ext is desired image extension. E.g: png, jpg.
	// bottom of cube: bottom.ext
	// top of cube: top.ext
	// left of cube: left.ext
	// right of cube: right.ext
	// front of cube: front.ext
	// back of cube: back.ext
	// eg: AddProgramFromExts({"front.ext", "bar.ext"});
	void AddUniqueTextures(const std::vector<std::string> a_texturePaths, const unsigned int a_gl_textureSlot);

	void Render();
private:
	//#TODO: Change names
	std::string m_name;
	unsigned int m_GLTexture;
	unsigned int m_GLSlot;
	unsigned int m_UniformLocation;
};