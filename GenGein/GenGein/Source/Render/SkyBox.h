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

	enum class SkyTheme : char
	{
		SKY,
		SPACE,
		CHAPEL,
		GOLDRUSH,
		FROZEN,
		MORNING,
		TROPICALSUNNY,
		THICKCLOUDSWATER,
		SUNSET,
		DARKSTORMY,
		CLOUDYLIGHTRAYS,
		_SIZE
	};

	SkyBox();
	SkyBox(const unsigned int*);
	~SkyBox();

	void Create(const SkyTheme);
	void Render();

private:
	const unsigned int* m_pSkyShader;
	Shape * m_pShape;
	TextureCube* m_pTextureCube;
};
