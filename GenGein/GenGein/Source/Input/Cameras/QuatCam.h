#pragma once
#include <glm\gtx\quaternion.hpp>

#include "BaseCam.h"

////
// Author: Jackson Luff
// Name: Fly QuadCam
// Type: class (standard)
// Parent: Base QuadCam
// Description:
// * The fly QuadCam is your typical 'spectator' 
// * QuadCam. You can fly around freely throughout
// * the scene.
////
class QuatCam : public BaseCam
{
public:
	// Constructors
	QuatCam();
	QuatCam(const glm::vec3 a_pos, const glm::vec3 a_lkAt,
		const float a_moveSpeed, const float a_rotationSpeed);

	// Deconstructor
	~QuatCam();

	// Overridden update
	virtual void Update(const double a_dt) override;

	// Get the fly speed
	inline const float GetFlySpeed() const {
		return m_flySpeed;
	}
	// Set the fly speed
	inline void SetFlySpeed(const float a_fSpeed) {
		m_flySpeed = a_fSpeed;
	}

private:
	// Input Handles
	void HandleKeyboardInput(const double a_dt);
	void HandleMouseInput(const double a_dt);
	// Calculate the rotation of the QuadCam
	void CalculateRotation(const double a_dt, glm::vec3 a_aircraftaxis);

	glm::quat m_camera_quat;
	glm::vec3 m_aircraft_axis;

	// Movement Speed
	float m_flySpeed;
	// Rotational speed
	float m_rotSpeed;
	// Movement Speed Multiplier (on Zoom)
	float m_flyMultiplier;
	// Mouse is down boolean
	bool m_bViewButtonClicked;
};