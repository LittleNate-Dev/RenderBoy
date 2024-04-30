/*
* Class:
*	DirectionalLight
* Description:
*	This class represents directional light
*/

#pragma once

#include "core/Core.h"

class DirectionalLight
{
private:
	std::string m_Name;
	glm::vec3 m_EulerAngle;
	glm::vec3 m_Color;
	// Parameters used for Bling-Fong shading
	glm::vec3 m_ADS; // Ambient, Diffuse, Specular
	float m_Intensity;
	// Turn on/off the light
	bool m_LightSwitch;
	// Show light cube
	bool m_ShowCube;
	// Whether the light casts shadow
	bool m_CastShadow;
	// Resolution of the shadow map
	unsigned int m_ShadowRes;

public:
	DirectionalLight();
	~DirectionalLight();

	// Get spot light's matrices
	glm::vec3 GetDirection();
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();
	glm::mat4 GetRotateMat();
	glm::mat4 GetModelMat();
	// Set spot light's attributes
	void SetName(std::string name);
	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetRoll(float roll);
	void SetEulerAngle(float pitch, float yaw, float roll);
	void SetEulerAngle(glm::vec3 eulerAngle);
	void SetColor(glm::vec3 color);
	void SetAmbient(float ambient);
	void SetDiffuse(float diffuse);
	void SetSpecular(float specular);
	void SetADS(float ambient, float diffuse, float specular);
	void SetADS(glm::vec3 ads);
	void SetIntensity(float intensity);
	void SetShowCube(bool drawCube);
	void SetLightSwitch(bool lightSwitch);
	void SetCastShadow(bool castShadow);
	void SetShadowRes(unsigned int res);
	// Get spot light's members
	std::string GetName();
	glm::vec3 GetEulerAngle();
	glm::vec3 GetColor();
	float GetAmbient();
	float GetDiffuse();
	float GetSpecular();
	glm::vec3 GetADS();
	float GetIntensity();
	bool ShowCube();
	bool LightSwitch();
	bool CastShadow();
	unsigned int GetShadowRes();
	// Draw UI
	void DrawUI();
};