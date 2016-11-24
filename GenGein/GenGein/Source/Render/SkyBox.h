#pragma once
#include <string>
#include <vector>
#include <glm\fwd.hpp>

class ShaderSet;
class Shape;
class TextureCube;

class SkyBox
{
public:

	enum class SkyType : char
	{
		SKY,
		SPACE,
		CHAPEL,
		GOLDRUSH,
		FROZEN,
		SIZE
	};

	SkyBox();
	SkyBox(const unsigned int* a_program);
	~SkyBox();

	void Create(const SkyType a_presetType);
	void Render();

private:
	const unsigned int* m_pSkyShader;
	Shape * m_pShape;
	TextureCube* m_pTextureCube;
};
