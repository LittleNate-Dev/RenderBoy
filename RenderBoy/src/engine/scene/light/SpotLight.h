/*
* Class:
*	SpotLight
* Description:
*	This class represents spot light
*/

#pragma once

#include "core/Core.h"

class SpotLight
{
private:
	std::string m_Name;
	glm::vec3 m_Position;
	glm::vec3 m_EulerAngle;
	glm::vec3 m_Color;
	// Spot light's beam angle
	float m_Angle;
	float m_DimAngle;
	// Parameters used for Bling-Fong shading
	glm::vec3 m_ADS; // Ambient, Diffuse, Specular
	// Parameters used for light attenuation
	float m_Range;
	float m_Intensity;
	glm::vec3 m_CLQ; // Constant, Linear and Quadratic
	// Turn on/off the light
	bool m_LightSwitch;
	// Show light cube
	bool m_ShowCube;
	// Whether the light casts shadow
	bool m_CastShadow;
	// Resolution of the shadow map
	unsigned int m_ShadowRes;

public:
	SpotLight();
	~SpotLight();

	// Get spot light's matrices
	glm::vec3 GetDirection();
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();
	glm::mat4 GetTranslateMat();
	glm::mat4 GetRotateMat();
	glm::mat4 GetModelMat();
	// Set spot light's attributes
	void SetName(std::string name);
	void SetPosition(float posX, float posY, float posZ);
	void SetPosition(glm::vec3 position);
	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetRoll(float roll);
	void SetEulerAngle(float pitch, float yaw, float roll);
	void SetEulerAngle(glm::vec3 eulerAngle);
	void SetColor(glm::vec3 color);
	// Angle + DimAngle is no greater than 180.0f
	void SetAngle(float angle);
	void SetDimAngle(float dimAngle);
	void SetAmbient(float ambient);
	void SetDiffuse(float diffuse);
	void SetSpecular(float specular);
	void SetADS(float ambient, float diffuse, float specular);
	void SetADS(glm::vec3 ads);
	void SetRange(float range);
	void SetIntensity(float intensity);
	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	void SetCLQ(float constant, float linear, float quadratic);
	void SetCLQ(glm::vec3 clq);
	void SetShowCube(bool drawCube);
	void SetLightSwitch(bool lightSwitch);
	void SetCastShadow(bool castShadow);
	void SetShadowRes(unsigned int res);
	// Get spot light's members
	std::string GetName();
	glm::vec3 GetPosition();
	glm::vec3 GetEulerAngle();
	glm::vec3 GetColor();
	float GetAngle();
	float GetDimAngle();
	float GetAmbient();
	float GetDiffuse();
	float GetSpecular();
	glm::vec3 GetADS();
	float GetRange();
	float GetIntensity();
	float GetConstant();
	float GetLinear();
	float GetQuadratic();
	glm::vec3 GetCLQ();
	bool ShowCube();
	bool LightSwitch();
	bool CastShadow();
	unsigned int GetShadowRes();
	// Draw UI
	void DrawUI();
};