#pragma once

#include "BaseCam.h"

////
// Author: Jackson Luff
// Name: Fly Camera
// Type: class (standard)
// Parent: Base Camera
// Description:
// * The fly camera is your typical 'spectator' 
// * camera. You can fly around freely throughout
// * the scene.
////
class FlyCam : public BaseCam
{
public:
	// Constructors
	FlyCam();
	FlyCam(const glm::vec3 a_pos, const glm::vec3 a_lkAt, 
		const float a_moveSpeed, const float a_rotationSpeed);

	// Deconstructor
	~FlyCam();

	// Overridden update
	virtual void Update(const double a_dt) override;
	
	// Get the fly speed
	inline const float GetFlySpeed() const
		{ return m_flySpeed; }
	// Set the fly speed
	inline void SetFlySpeed(const float a_fSpeed)
		{ m_flySpeed = a_fSpeed; }

private:
	// Input Handles
	void HandleKeyboardInput(const double a_dt);
	void HandleMouseInput(const double a_dt);
	// Calculate the rotation of the camera
	void CalculateRotation(const double a_dt, const glm::dvec2 a_cursorDelta);

	// Movement Speed
	float m_flySpeed;
	// Rotational speed
	float m_rotSpeed;
	// Mouse is down boolean
	bool m_bViewButtonClicked;
};