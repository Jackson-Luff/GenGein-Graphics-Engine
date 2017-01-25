#pragma once
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(const glm::vec3 a_colour = glm::vec3(1), float a_intensity = 0.0f);

	virtual ~Light();

	// Get | Set Shader program
	inline const unsigned int* const GetShader() { return m_shader; }
	void SetShader(unsigned int* m_shader);
private:
	unsigned int* m_shader;

	float m_intensity;
	glm::vec3 m_colour;
};

////
// Varied lighting types [i.e Directional, Spot and Point]
////

//
struct DirectionalLight : public Light
{
	// Constructor
	DirectionalLight(const glm::vec3 a_colour = glm::vec3(1), float a_intensity = 0.0f);
};

//
struct PointLight : public Light
{
	// Properties
	struct Attenuation
	{
		Attenuation(float a_constant = 0, float a_linear = 0, float a_exponent = 1)
			: constant(a_constant), linear(a_linear), exponent(a_exponent) {}

		float constant, linear, exponent;
	} atten;
	float range;
	
	// Constructor
	PointLight(const glm::vec3 a_colour = glm::vec3(1), float a_intensity = 0.0f,
		const Attenuation& a_atten = Attenuation());
};

//
struct SpotLight : public PointLight
{
	// Properties
	float cutoff;

	// Constructor
	SpotLight(const glm::vec3 a_colour = glm::vec3(1), float a_intensity = 0.0f,
		const Attenuation a_atten = Attenuation(), float a_cutoff = 0.0);
};