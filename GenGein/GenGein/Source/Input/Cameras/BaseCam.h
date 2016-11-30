#pragma once 
#include <glm\glm.hpp>

// GLM definitions
//#TODO:FORWARD DECLARE GLM FOR .LIB BUILD

////
// Author: Jackson Luff
// Name: Base Camera
// Type: class (standard)
// Parent: None
// Description:
// * The BaseCamera will be the 'data container' that
// * varied camera types will inherit. This class allows
// * the user to access non-specific camera information.
////
class BaseCam
{
public:
	// Constructors
	BaseCam();
	BaseCam(const glm::vec3& a_position, const glm::vec3& a_lkAt);
	BaseCam(const glm::mat4& a_transform);

	// Deconstructor
	~BaseCam();

	// Inheritable update
	virtual void Update(const double a_dt){};

	// Get Position
	inline const glm::vec4& GetPosition() const { return m_worldTrans[3]; }
	// Set Position
	void SetPosition(const glm::vec4& a_position);

	// Get Current Speed
	inline const float GetBaseSpeed() const { return m_currSpeed; }
	// Set Current Speed
	inline float SetBaseSpeed(const float a_newSpeed) { return m_currSpeed = a_newSpeed; }

	// Multiple LookAt functions for varied arguments
	void LookAt(const glm::vec3 a_lookAt);
	void LookAt(const glm::vec3 a_position, const glm::vec3 a_lookAt);

	// Setup the perspective of the camera
	void SetPerspective(const float a_FOV, const float a_aspectRatio);

	// Sends out a ray cast from camera's direction
	glm::vec3 ScreenPosToDir(const float x, const float y);
	// Sends a ray out to a desired plane
	glm::vec3 PickAgainstPlane(const glm::vec4& plane) const;

	// Getter for Projection Transform
	inline const glm::mat4& GetProjTrans()		const { return m_projTrans; }

	// Getter for View Transform
	inline const glm::mat4& GetViewTrans()		const { return m_viewTrans; }

	// Getter for ProjectionView Transform
	inline const glm::mat4& GetProjViewTrans()	const { return m_projViewTrans; }

	// Getter for World Position
	inline const glm::mat4& GetWorldTrans()		const { return m_worldTrans; }
	// Setter for World Transform
	void SetWorldTrans(const glm::mat4& a_newWrldTrns);

protected:
	// Update Projection View
	void UpdateProjViewTrans();

	// Camera's internal transforms
	glm::mat4 m_viewTrans;
	glm::mat4 m_projTrans;
	glm::mat4 m_projViewTrans;

	// Camera's world transform
	glm::mat4 m_worldTrans;

	// Stores current speed from all camera's
	float m_currSpeed;
};