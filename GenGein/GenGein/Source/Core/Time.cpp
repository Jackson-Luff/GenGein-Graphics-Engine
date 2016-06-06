#include <GLFW\glfw3.h>
#include <math.h>
#include "Time.h"

Time::Time() :
m_deltaTime(0.0),
m_elapsedTime(0.001),
m_prevTime(0.0),
m_deltaTimeType(DT_DYNAMIC)
{}

Time::Time(const DeltaTimeID a_presetDeltaTime) :
m_deltaTime(0.0),
m_elapsedTime(0.001),
m_prevTime(0.0)
{
	m_deltaTimeType = a_presetDeltaTime;
	m_deltaTime = a_presetDeltaTime;
}

void Time::Update()
{
	switch (m_deltaTimeType)
	{
	case Time::DT_FIXED:
		m_deltaTime = 1 / 60.0f;
		break;
	case Time::DT_DYNAMIC:
		VariedDeltaTime();
		break;
	case Time::DT_SEMI_FIXED:
		SemiFixedDeltaTime();
		break;
	case Time::DT_BETTER:
		BetterDeltaTime();
		break;
	case Time::DT_BEST:
		BestDeltaTime();
		break;
	default:
		break;
	}

	m_elapsedTime = glfwGetTime();
}

// Private Functions:

void Time::VariedDeltaTime()
{
	double currTime = glfwGetTime();
	m_deltaTime = currTime - m_prevTime;
	m_prevTime = currTime;
}

void Time::SemiFixedDeltaTime()
{
	double time = 0.0f;
	double dt = 1 / 60.0f;
	double currTime = glfwGetTime();
	double frameTime = currTime - m_prevTime;

	m_deltaTime = (dt + frameTime) / 2.0f;

	m_prevTime = currTime;
}

void Time::BetterDeltaTime()
{

}

void Time::BestDeltaTime()
{

}