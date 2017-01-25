#include "Light.h"
#include <glm\detail\func_common.hpp>

#define COLOUR_DEPTH 256

Light::Light() : 
m_colour(glm::vec3(1)), m_intensity(1.0f)
{}

Light::Light(const glm::vec3 a_colour, float a_intensity)
{
	m_colour = a_colour;
	m_intensity = a_intensity;
}

Light::~Light()
{

}

void Light::SetShader(unsigned int* a_shader)
{
	// #TODO: to clean up old shader
	m_shader = a_shader;
}

DirectionalLight::DirectionalLight(const glm::vec3 a_colour, float a_intensity) :
	Light(a_colour, a_intensity)
{
	// #TODO: make shader gets/sets
	SetShader(0);
}

PointLight::PointLight(const glm::vec3 a_colour, float a_intensity,
	const Attenuation& a_atten)
	: Light(a_colour, a_intensity), atten(a_atten)
{
	float a = atten.exponent;
	float b = atten.linear;
	float c = atten.constant - COLOUR_DEPTH * a_intensity; // * a_colour.max();

	range = (-b + glm::sqrt(b*b - 4 * a * c)) / (2 * a);

	// #TODO: make shader gets/sets
	SetShader(0);
}

SpotLight::SpotLight(const glm::vec3 a_colour, float a_intensity,
	const Attenuation a_atten, float a_cutoff) :
	PointLight(a_colour, a_intensity, a_atten),
	cutoff(a_cutoff)
{
	SetShader(0);
}