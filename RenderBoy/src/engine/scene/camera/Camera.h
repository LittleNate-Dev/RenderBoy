/*
* Class:
*	Camera
* Description:
*	I'm pretty sure you know what this class is.
*/

#pragma once

#include "core/Core.h"

class Camera
{
private:
	// True: Perspective 
	// False: Orthographic
	// Default: Perspective
	bool m_Type;
	// window's size
	glm::vec2 m_WindowSize;
	float m_FOV;
	// first is near plane, and second is far plane
	glm::vec2 m_Plane;
	glm::vec3 m_Position;
	glm::vec3 m_EulerAngle;
	float m_MoveSpeed;
	float m_RotateSpeed;

public:
	Camera();
	~Camera();

	// Set Camera's parameter
	// True:: Perspective False: Orthographic
	void SetCameraType(bool type);
	void SetWindowSize();
	void SetWindowSize(unsigned int width, unsigned int height);
	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetPlane(float nearPlane, float farPlane);
	void SetPlane(glm::vec2 plane);
	void SetPosition(float posX, float posY, float posZ);
	void SetPosition(glm::vec3 position);
	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetRoll(float roll);
	void SetEulerAngle(float pitch, float yaw, float roll);
	void SetEulerAngle(glm::vec3 eulerAngle);
	void SetMoveSpeed(float speed);
	void SetRotateSpeed(float speed);
	// Get Camera's matrices
	glm::vec3 GetDirection(glm::vec3 direction);
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();
	// Get Camera's members
	bool GetCameraType();
	glm::vec2 GetWindowSize();
	float GetFOV();
	float GetNearPlane();
	float GetFarPlane();
	glm::vec2 GetPlane();
	glm::vec3 GetPosition();
	glm::vec3 GetEulerAngle();
	float GetMoveSpeed();
	float GetRotateSpeed();
	// Draw UI
	void DrawUI();
};