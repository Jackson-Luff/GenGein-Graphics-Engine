#include <glm\gtx\rotate_vector.hpp>
#include "Input\Input.h"
#include "Input\Console\Console.h"
#include "QuatCam.h"

using Input::Keyboard;
using Input::Cursor;
using Input::Window;

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


QuatCam::QuatCam() :
	m_flySpeed(0),
	m_rotSpeed(0),
	m_bViewButtonClicked(false)
{}

QuatCam::QuatCam(const vec3 a_pos, const vec3 a_lkAt,
	const float a_flySpeed, const float a_rotSpeed)
	: BaseCam(a_pos, a_lkAt)
{
	m_flySpeed = a_flySpeed;
	m_rotSpeed = a_rotSpeed;
	SetBaseSpeed(m_flySpeed);

	// Initialise data
	m_bViewButtonClicked = false;
}

QuatCam::~QuatCam()
{}

void QuatCam::Update(const double a_dt)
{
	// Update the handling queries
	HandleKeyboardInput(a_dt);
	HandleMouseInput(a_dt);
}

// Private Functions:

void QuatCam::HandleKeyboardInput(const double a_dt)
{
	//Get the QuatCams forward/up/right
	vec3 vRight = vec3(m_worldTrans[0]);
	vec3 vUp = vec3(m_worldTrans[1]);
	vec3 vFrwrd = vec3(m_worldTrans[2]);

	vec3 moveDir(0.0f);

	//Retain a direction via with button is pressed
	if (Keyboard::isKeyDown(KEY_W))
		moveDir -= vFrwrd;
	else if (Keyboard::isKeyDown(KEY_S))
		moveDir += vFrwrd;
	else if (Keyboard::isKeyDown(KEY_A))
		moveDir -= vRight;
	else if (Keyboard::isKeyDown(KEY_D))
		moveDir += vRight;

	if (Keyboard::isKeyDown(KEY_E))
		moveDir += vec3(0.0f, 1.0f, 0.0f);
	else if (Keyboard::isKeyDown(KEY_Q))
		moveDir -= vec3(0.0f, 1.0f, 0.0f);

	m_flySpeed = GetBaseSpeed();

	float vel = 10.0f;
	float maxSpeed = 25.0f;

	// Applying speed boosts
	if (Keyboard::isKeyDown(KEY_LEFT_SHIFT))
		SetFlySpeed(vel * m_flySpeed);
	else if (m_flySpeed >= maxSpeed)
		SetFlySpeed(m_flySpeed * 0.98f);

	if (m_flySpeed <= 0.1f)
		SetFlySpeed(0.0f);

	//Apply movement to the current position
	if (length(moveDir) > 0.01f)
	{
		moveDir = ((float)a_dt * m_flySpeed) * normalize(moveDir);
		SetPosition(GetPosition() + vec4(moveDir, 0.0));
	}
}

void QuatCam::HandleMouseInput(const double a_dt)
{
	// Check for Right mouse key clicked
	if (Cursor::isRightButtonDown())
	{
		// Check for held down
		if (m_bViewButtonClicked == false)
		{
			glm::ivec2 winSize = Window::GetWindowSize();
			glm::dvec2 screenCenter = glm::dvec2(winSize.x, winSize.y) / 2.0;

			Cursor::SetOldCursorPos(screenCenter);
			Cursor::SetCursorPos(screenCenter);

			m_bViewButtonClicked = true;
		}
		else
		{
			// Retrieve current mouse position
			glm::dvec2 newCurPos = Cursor::GetCursorPos();

			//Calculate delta between new and old mouse position
			glm::dvec2 delta = newCurPos - Cursor::GetOldCursorPos();

			//Aircraft axis
			const float sensitivity = 0.0020f;
			glm::vec3 aircraft_axis;
			m_aircraft_axis.y = (float)((a_dt * -m_rotSpeed) * delta.x);
			m_aircraft_axis.x = (float)((a_dt * -m_rotSpeed) * delta.y);

			// Save off previous cursor position
			Cursor::SetOldCursorPos(newCurPos);

			// Calculate the rotation
			CalculateRotation(a_dt, m_aircraft_axis);
		}
	}
	else
		m_bViewButtonClicked = false;

}

void QuatCam::CalculateRotation(const double a_dt, glm::vec3 a_aircraftAxis)
{
	// Calculate the rotation of the delta vector
	glm::quat key_quat = glm::quat(a_aircraftAxis);

	//if (glm::length(a_aircraftAxis) > 0.01f)
	m_camera_quat = key_quat * m_camera_quat;
	m_camera_quat = glm::normalize(m_camera_quat);
	glm::mat4 rotate = glm::mat4_cast(m_camera_quat);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -glm::vec3(m_worldTrans[1]));
	m_viewTrans = rotate * translate;

	SetWorldTrans(m_worldTrans * mat4(glm::normalize(key_quat)));
	
	//mat3 xRot = mat3(rotate((float)(a_cursorDelta.x * (a_dt * -m_rotSpeed)), vec3(0, 1, 0)));
	//mat3 yRot = mat3(rotate((float)(a_cursorDelta.y * (a_dt * -m_rotSpeed)), vec3(1, 0, 0)));
	//
	//// Make sure the magnitude is larger than 0 (divide by 0 error)
	//if (length(a_cursorDelta) > 0.01f)
	//	SetWorldTrans(m_worldTrans * mat4(xRot * yRot));
	//
	//// Create a side vector and then from that an up vector 
	//vec3 m_sideVector = cross(vec3(0, 1, 0), vec3(m_worldTrans[2]));
	//vec3 m_upVector = cross(m_sideVector, vec3(m_worldTrans[2]));
	//
	//// Normalise the two
	//m_sideVector = normalize(m_sideVector);
	//m_upVector = normalize(m_upVector);
	//
	//// Apply to the world Transform 
	//m_worldTrans[0] = vec4(m_sideVector, 0);
	//m_worldTrans[1] = vec4(-m_upVector, 0);
	//m_worldTrans[2] = normalize(m_worldTrans[2]);
	//// Apply View Transform 
	//m_viewTrans = glm::inverse(m_worldTrans);
}