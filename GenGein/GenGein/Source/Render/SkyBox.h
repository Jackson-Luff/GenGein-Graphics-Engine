#pragma once
#include <string>
#include <vector>
#include <glm\fwd.hpp>

class SkyBox
{
public:

	enum class SkyType
	{
		SKY = 1,
		SPACE = 2,
		CHAPEL = 3,
		SIZE
	};

	SkyBox();
	~SkyBox();

	void Create(const SkyType& a_presetType);
	void Render(const glm::mat4& a_proj, const glm::mat4& a_view);

private:
	void InitialiseDirs(const SkyType& a_presetType);
	void LoadShaderProgram();
	void LoadCubeVertices();
	void LoadCubeTextures(const std::vector<std::string>& a_dirs);
	
	unsigned int m_vao, m_vbo;
	unsigned int m_skyShaderID;
	unsigned int m_cubeMapID;

	// uniform shit
	unsigned int m_cubemapUniLoc;

	std::vector<std::string> m_presetDirectires;
};
