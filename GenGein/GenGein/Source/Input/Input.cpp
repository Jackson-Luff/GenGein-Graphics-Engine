#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "Input\Cameras\BaseCam.h"

#include "Input.h"

using Input::Keyboard;
using Input::Cursor;
using Input::Window;
using Input::Camera;
using Input::Time;

// Keyboard

bool Keyboard::isKeyUp(const int a_key)
{
	if (!glfwGetKey(glfwGetCurrentContext(), (int)a_key))
		return true;

	return false;
}

bool Keyboard::isKeyDown(const int a_key)
{
	if (glfwGetKey(glfwGetCurrentContext(), (int)a_key))
		return true;

	return false;
}

bool Keyboard::isKeyRelease(const int a_key)
{
	return false;
}

void Keyboard::RunDebugControls()
{
	if (isKeyDown(KEY_GRAVE_ACCENT))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (isKeyDown(KEY_1))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (isKeyDown(KEY_2))
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
// Cursor

glm::dvec2 Cursor::m_oldCursorPos = glm::dvec2(0);

bool Cursor::isLeftButtonDown()
{
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_1))
		return true;

	return false;
}

bool Cursor::isRightButtonDown()
{
	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_2))
		return true;

	return false;
}

glm::dvec2 Cursor::GetCursorPos()
{
	double x, y;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	return glm::dvec2(x, y);
}

glm::dvec2 Cursor::GetOldCursorPos()
{
	return m_oldCursorPos;
}

void Cursor::SetCursorPos(const glm::dvec2 a_newCurPos)
{
	glfwSetCursorPos(glfwGetCurrentContext(), a_newCurPos.x, a_newCurPos.y);
}

void Cursor::SetOldCursorPos(const glm::dvec2 a_newCurPos)
{
	m_oldCursorPos = a_newCurPos;
}

// Window

glm::ivec2 Window::GetWindowSize()
{
	int width, height;
	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	return glm::ivec2(width, height);
}

// Camera

BaseCam* Camera::m_cam = new BaseCam();

const glm::vec4 Camera::GetRight()
{
	return m_cam->GetWorldTrans()[0];
}

const glm::vec4 Camera::GetUp()
{
	return m_cam->GetWorldTrans()[1];
}

const glm::vec4 Camera::GetForward()
{
	return m_cam->GetWorldTrans()[2];
}

const glm::vec4 Camera::GetCamPos()
{
	return glm::vec4(m_cam->GetPosition());
}

void Camera::SetCamPos(glm::vec4 a_pos)
{
	m_cam->SetPosition(a_pos);
}

const glm::mat4 Camera::GetCameraWorldTrans()
{
	return m_cam->GetWorldTrans();
}

// Time

double* Time::m_elaspedTime;
double* Time::m_deltaTime;