#include <glm\ext.hpp>
#include "Input\Input.h"

#include "BaseCam.h"

using Input::Keyboard;
using Input::Cursor;
using Input::Window;

BaseCam::BaseCam()
	: m_viewTrans(glm::mat4(1)),
	m_projTrans(glm::mat4(1)),
	m_worldTrans(glm::mat4(1)),
	m_currSpeed(0)
{}

BaseCam::BaseCam(const glm::vec3& a_pos, const glm::vec3& a_lkAt) 
	: BaseCam()
{
	// Set Position
	SetPosition(glm::vec4(a_pos,1.0f));
	// Set LookAt
	LookAt(a_lkAt);
	// Defaulted Perspective
	SetPerspective(glm::pi<float>()*0.375f, 16.0f / 9.0f);

	// Update the projection view 
	UpdateProjViewTrans();
}

BaseCam::BaseCam(const glm::mat4& a_trans)
{
	// Set curr speed;
	m_currSpeed = 0;
	// Set the world transform
	m_worldTrans = a_trans;
	// Update the projection view 
	UpdateProjViewTrans();
}

BaseCam::~BaseCam()
{}

void BaseCam::SetWorldTrans(const glm::mat4& a_newWrldTrns)
{
	// Set the world transform
	m_worldTrans = a_newWrldTrns;
	// Update the projection view 
	UpdateProjViewTrans();
}

void BaseCam::SetPosition(const glm::vec4& a_pos)
{
	// Set Position within world
	m_worldTrans[3] = a_pos;
	// Update the projection view
	UpdateProjViewTrans();
}

void BaseCam::LookAt(const glm::vec3 a_lkAt)
{
	// Apply LookAt without a position
	m_worldTrans = inverse(lookAt(glm::vec3(m_worldTrans[3]), a_lkAt, glm::vec3(0, 1, 0)));
	// Update the projection view 
	UpdateProjViewTrans();
}

void BaseCam::LookAt(const glm::vec3 a_pos, const glm::vec3 a_lkAt)
{
	// Apply LookAt with a position
	m_worldTrans = inverse(lookAt(a_pos, a_lkAt, glm::vec3(0, 1, 0)));
	// Update the projection view 
	UpdateProjViewTrans();
}

void BaseCam::SetPerspective(const float a_FOV, const float a_aspectRatio)
{
	// Initialise the projection transform 
	m_projTrans = glm::perspective(a_FOV, a_aspectRatio, 0.1f, 10000.0f);
	// Update the projection view
	UpdateProjViewTrans();
}

glm::vec3 BaseCam::ScreenPosToDir(const float x, const float y)
{
	glm::int2 winSize = Window::GetWindowSize();
	glm::vec3 screenPos(x / winSize.x * 2 - 1, (y / winSize.y * 2 - 1) * -1, -1);

	screenPos.x /= m_projTrans[0][0];
	screenPos.y /= m_projTrans[1][1];

	return glm::vec3(normalize(m_worldTrans * glm::vec4(screenPos, 0)));
}

glm::vec3 BaseCam::PickAgainstPlane(const glm::vec4& plane) const
{
	// Extract cursor position
	glm::dvec2 curPos = Cursor::GetCursorPos();
	// Extract window measurements
	glm::ivec2 winSize = Window::GetWindowSize();
	
	// Convert cursor position into OpenGL coordinates (-1 to 1)
	glm::vec3 screenPos(curPos.x / winSize.x * 2 - 1, (curPos.y / winSize.y * 2 - 1) * -1, -1);

	//
	screenPos.x /= m_projTrans[0][0];
	screenPos.y /= m_projTrans[1][1];

	//
	glm::vec3 dir = glm::vec3(normalize(m_worldTrans * glm::vec4(screenPos, 0)));

	// Line-Plane Intersection method:
	// Reference: http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	// (P - Po) • N = 0
	// ((dL + Lo) - Po) • N = 0
	// ((dL + 'CamPos') - ('0,0.73,0')) • N = 0
	// (dL)• N + ('CamPos' - Po) • N = 0
	// d = (Po - Lo) • N / L • N
	// d = ((plane.xyz() * plane.w) - 'CamPos') • plane.xyz() / dir • plane.xyz()
	float PoSubLoDotN = dot((glm::vec3(plane) * plane.w) - glm::vec3(m_worldTrans[3]), glm::vec3(plane));
	float LDotN = dot(dir, glm::vec3(plane));
	// thus:
	float d = PoSubLoDotN / LDotN;

	// 
	return glm::vec3(m_worldTrans[3]) + dir * d;
}

// Protected functions:

void BaseCam::UpdateProjViewTrans()
{
	// Inverse the world transform to get the view transform
	m_viewTrans = inverse(m_worldTrans);
	// Apply the projection and view transform into the projectionView transform
	m_projViewTrans = m_projTrans * m_viewTrans;
}
