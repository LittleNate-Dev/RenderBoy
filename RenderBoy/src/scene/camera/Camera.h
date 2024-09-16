/*
* Class:
*	Camera
* Description:
*	I'm pretty sure you know what this class is.
*/

#pragma once

#include "core/Core.h"

struct VFX_Bloom
{
	bool Switch = false;
	float Strength = 0.04f;
	float FilterRadius = 0.005f;
};

struct VFX_Focus
{
	bool Switch = true;
	float Distance;
	float Range;
	float FocalLength = 8.0f;
};

struct VFX_Exposure
{
	bool Auto = true;
	float Strength = 1.0f;
};

struct VFX
{
	VFX_Bloom Bloom;
	VFX_Focus Focus;
	VFX_Exposure Exposure;
};

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
	VFX m_VFX;

public:
	Camera();
	~Camera();

	void Reset();
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
	// Camera VFX
	inline void SetBloomSwitch(bool bloomSwitch) 
	{
		m_VFX.Bloom.Switch = bloomSwitch;
	};
	void SetBloomStrength(float strength);
	void SetBloomFilterRadius(float radius);
	inline void SetFocusSwitch(bool focusSwitch)
	{
		m_VFX.Focus.Switch = focusSwitch;
	};
	void SetFocusDistance(float distance);
	void SetFocusRange(float range);
	void SetFocalLength(float length);
	inline void SetAutoExposure(bool autoEx)
	{
		m_VFX.Exposure.Auto = autoEx;
	};
	void SetExposure(float exposure);
	
	// Get Camera's matrices
	glm::vec3 GetDirection(glm::vec3 direction);
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();
	glm::mat4 GetProjMat(float start, float end); // To get part of the camera's view frustum
	// Get Camera's members
	inline bool GetCameraType() const
	{
		return m_Type;
	};
	inline glm::vec2 GetWindowSize() const
	{
		return m_WindowSize;
	};
	inline float GetFOV() const
	{
		return m_FOV;
	};
	inline float GetNearPlane() const
	{
		return m_Plane.x;
	};
	inline float GetFarPlane() const
	{
		return m_Plane.y;
	};
	inline glm::vec2 GetPlane() const
	{
		return m_Plane;
	};
	inline glm::vec3 GetPosition() const
	{
		return m_Position;
	};
	inline glm::vec3 GetEulerAngle() const
	{
		return m_EulerAngle;
	};
	inline float GetMoveSpeed() const
	{
		return m_MoveSpeed;
	};
	inline float GetRotateSpeed() const
	{
		return m_RotateSpeed;
	};
	inline VFX_Focus GetFocus() const
	{
		return m_VFX.Focus;
	};
	inline VFX_Bloom GetBloom() const 
	{
		return m_VFX.Bloom;
	};
	inline VFX_Exposure GetExposure() const 
	{
		return m_VFX.Exposure;
	};
	// Draw UI
	void DrawUI();
};