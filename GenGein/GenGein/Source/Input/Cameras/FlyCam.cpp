#include <glm\gtx\rotate_vector.hpp>
#include "Input\Input.h"
#include "Input\Console\Console.h"
#include "FlyCam.h"

using Input::Keyboard;
using Input::Cursor;
using Input::Window;

using glm::ivec2;
using glm::dvec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

FlyCam::FlyCam() :
m_rotSpeed(0),
m_flyVelocity(vec3(0)),
m_bViewButtonClicked(false)
{}

FlyCam::FlyCam(const vec3 a_pos, const vec3 a_lkAt,
	const float a_flySpeed, const float a_rotSpeed) : BaseCam(a_pos, a_lkAt)
{
	SetBaseSpeed(a_flySpeed);
	m_rotSpeed = a_rotSpeed;
	m_flyVelocity = vec3(0.0f);
	

	// Initialise data
	m_bViewButtonClicked = false;
}

FlyCam::~FlyCam()
{}

void FlyCam::Update(const double a_dt)
{
	// Update the handling queries
	HandleKeyboardInput(a_dt);
	HandleMouseInput(a_dt);
}

// Private Functions:

void FlyCam::HandleKeyboardInput(const double a_dt)
{
	//Get the cameras forward/up/right
	vec3 vRight	= vec3(m_worldTrans[0]);
	vec3 vUp	= vec3(m_worldTrans[1]);
	vec3 vFrwrd	= vec3(m_worldTrans[2]);
	
	vec3 moveDir = vec3(0.0);

	//Retain a direction via with button is pressed
	if (Keyboard::isKeyDown(KEY_W))
		moveDir -= vFrwrd;
	if (Keyboard::isKeyDown(KEY_S))
		moveDir += vFrwrd;
	if (Keyboard::isKeyDown(KEY_A))
		moveDir -= vRight;
	if (Keyboard::isKeyDown(KEY_D))
		moveDir += vRight;
	
	if (Keyboard::isKeyDown(KEY_E))
		moveDir += vec3(0.0f, 1.0f, 0.0f);
	else if (Keyboard::isKeyDown(KEY_Q))
		moveDir -= vec3(0.0f, 1.0f, 0.0f);
	
	if (length(moveDir) > 0.0f)
		m_flyVelocity += moveDir;

	if (length(m_flyVelocity) > 0.0f)
	{		
		m_flyVelocity *= 0.985f;

		if (length(m_flyVelocity) > 7.5f)
			m_flyVelocity = 7.5f * normalize(m_flyVelocity);

		// Update apply velocity and speed relative to movement
		const float hyperSpeed = 8.0f;
		float velStr = ((float)a_dt * m_currSpeed);
		
		// If(Left_Shift) HYYYPPPPEEERRRSPEEEDD BOISSS
		if (Keyboard::isKeyDown(KEY_LEFT_SHIFT))
			velStr *= hyperSpeed;
				
		// Update render capture
		SetPosition(GetPosition() + vec4(m_flyVelocity * velStr, 0.0));
		UpdateProjViewTrans();
	}
	

}

void FlyCam::HandleMouseInput(const double a_dt)
{
	// Check for Right mouse key clicked
	if (Cursor::isRightButtonDown())
	{
		// Check for held down
		if (m_bViewButtonClicked == false)
		{
			ivec2 winSize = Window::GetWindowSize();
			dvec2 screenCenter = dvec2(winSize.x, winSize.y) / 2.0;

			Cursor::SetOldCursorPos(screenCenter);
			Cursor::SetCursorPos(screenCenter);

			m_bViewButtonClicked = true;
		}
		else
		{
			// Retrieve current mouse position
			dvec2 newCurPos = Cursor::GetCursorPos();

			//Calculate delta between new and old mouse position
			dvec2 delta = newCurPos - Cursor::GetOldCursorPos();

			// Save off previous cursor position
			Cursor::SetOldCursorPos(newCurPos);

			// Calculate the rotation
			CalculateRotation(a_dt, delta);
		}
	}
	else
		m_bViewButtonClicked = false;

}

void FlyCam::CalculateRotation(const double a_dt, const dvec2 a_cursorDelta)
{
	// Calculate the rotation of the delta vector
	mat3 xRot = mat3(rotate((float)(a_cursorDelta.x * (a_dt * -m_rotSpeed)), vec3(0, 1, 0)));
	mat3 yRot = mat3(rotate((float)(a_cursorDelta.y * (a_dt * -m_rotSpeed)), vec3(1, 0, 0)));

	// Make sure the magnitude is larger than 0 (divide by 0 error)
	if (length(a_cursorDelta) > 0.01f)
		SetWorldTrans(m_worldTrans * mat4(xRot * yRot));

	// Create a side vector and then from that an up vector 
	vec3 m_sideVector = cross(vec3(0, 1, 0), vec3(m_worldTrans[2]));
	vec3 m_upVector   = cross(m_sideVector,  vec3(m_worldTrans[2]));

	// Normalise the two
	m_sideVector = normalize(m_sideVector);
	m_upVector = normalize(m_upVector);

	// Apply to the world Transform 
	m_worldTrans[0] = vec4(m_sideVector, 0);
	m_worldTrans[1] = vec4(-m_upVector, 0);
	m_worldTrans[2] = normalize(m_worldTrans[2]);
	// Apply View Transform 
	m_viewTrans = inverse(m_worldTrans);
}