/*
* Class:
*	PointLight
* Description:
*	This class represents point light
*/

#pragma once

#include "core/Core.h"

class PointLight
{
private:
	std::string m_Name;
	glm::vec3 m_Position;
	glm::vec3 m_Color;
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
	PointLight();
	~PointLight();

	// Get spot light's matrices
	glm::mat4 GetViewMat(unsigned int face);
	glm::mat4 GetProjMat();
	glm::mat4 GetTranslateMat();
	glm::mat4 GetModelMat();
	// Set point light's attributes
	void SetName(std::string name);
	void SetPosition(float posX, float posY, float posZ);
	void SetPosition(glm::vec3 position);
	void SetColor(glm::vec3 color);
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
	glm::vec3 GetColor();
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