#include <glm\ext.hpp>
#include "Input\Input.h"

#include "BaseCam.h"

using Input::Keyboard;
using Input::Cursor;
using Input::Window;

using glm::vec3;
using glm::vec4;
using glm::mat4;

BaseCam::BaseCam()
	: m_viewTrans(mat4(1)),
	m_projTrans(mat4(1)),
	m_worldTrans(mat4(1)),
	m_currSpeed(0)
{}

BaseCam::BaseCam(const vec3& a_pos, const vec3& a_lkAt) 
	: BaseCam()
{
	// Set Position
	SetPosition(vec4(a_pos,1.0f));
	// Set LookAt
	LookAt(a_lkAt);
	// Defaulted Perspective
	SetPerspective(glm::pi<float>()*0.25f, 16.0f / 9.0f);

	// Update the projection view 
	UpdateProjViewTrans();
}

BaseCam::BaseCam(const mat4& a_trans)
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

void BaseCam::SetWorldTrans(const mat4& a_newWrldTrns)
{
	// Set the world transform
	m_worldTrans = a_newWrldTrns;
	// Update the projection view 
	UpdateProjViewTrans();
}

void BaseCam::SetPosition(const vec4& a_pos)
{
	// Set Position within world
	m_worldTrans[3] = a_pos;
	// Update the projection view
	UpdateProjViewTrans();
}

void BaseCam::LookAt(const vec3 a_lkAt)
{
	// Apply LookAt without a position
	m_worldTrans = inverse(lookAt(vec3(m_worldTrans[3]), a_lkAt, vec3(0, 1, 0)));
	// Update the projection view 
	UpdateProjViewTrans();
}

void BaseCam::LookAt(const vec3 a_pos, const vec3 a_lkAt)
{
	// Apply LookAt with a position
	m_worldTrans = inverse(lookAt(a_pos, a_lkAt, vec3(0, 1, 0)));
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

vec3 BaseCam::ScreenPosToDir(const float x, const float y)
{
	glm::ivec2 winSize = Window::GetWindowSize();
	vec3 screenPos(x / winSize.x * 2 - 1, (y / winSize.y * 2 - 1) * -1, -1);

	screenPos.x /= m_projTrans[0][0];
	screenPos.y /= m_projTrans[1][1];

	return vec3(normalize(m_worldTrans * vec4(screenPos, 0)));
}

vec3 BaseCam::PickAgainstPlane(const vec4& plane) const
{
	// Extract cursor position
	glm::dvec2 curPos = Cursor::GetCursorPos();
	// Extract window measurements
	glm::ivec2 winSize = Window::GetWindowSize();
	
	// Convert cursor position into OpenGL coordinates (-1 to 1)
	vec3 screenPos(curPos.x / winSize.x * 2 - 1, (curPos.y / winSize.y * 2 - 1) * -1, -1);

	//
	screenPos.x /= m_projTrans[0][0];
	screenPos.y /= m_projTrans[1][1];

	//
	vec3 dir = vec3(normalize(m_worldTrans * vec4(screenPos, 0)));

	// Line-Plane Intersection method:
	// Reference: http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
	// (P - Po) • N = 0
	// ((dL + Lo) - Po) • N = 0
	// ((dL + 'CamPos') - ('0,0.73,0')) • N = 0
	// (dL)• N + ('CamPos' - Po) • N = 0
	// d = (Po - Lo) • N / L • N
	// d = ((plane.xyz() * plane.w) - 'CamPos') • plane.xyz() / dir • plane.xyz()
	float PoSubLoDotN = dot((vec3(plane) * plane.w) - vec3(m_worldTrans[3]), vec3(plane));
	float LDotN = dot(dir, vec3(plane));
	// thus:
	float d = PoSubLoDotN / LDotN;

	// 
	return vec3(m_worldTrans[3]) + dir * d;
}

// Protected functions:

void BaseCam::UpdateProjViewTrans()
{
	// Inverse the world transform to get the view transform
	m_viewTrans = inverse(m_worldTrans);
	// Apply the projection and view transform into the projectionView transform
	m_projViewTrans = m_projTrans * m_viewTrans;
}
