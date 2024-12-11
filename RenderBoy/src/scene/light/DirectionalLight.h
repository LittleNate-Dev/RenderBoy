/*
* Class:
*	DirectionalLight
* Description:
*	This class represents directional light
*/

#pragma once

#include "core/Core.h"
#include "data/Data.h"

class DirectionalLight
{
private:
	std::string m_Name;
	glm::mat4 m_ProjMat[3];
	glm::mat4 m_ViewMat[3];
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
	glm::vec2 m_CSMRatio;
	float m_ShadowEnlarge;
	glm::vec3 m_Bias;
	bool m_SoftShadow;
	float m_SoftDegree;

public:
	DirectionalLight();
	~DirectionalLight();

	// Get spot light's matrices
	void UpdateShadowMat(std::vector<glm::mat4> cameraProj, glm::mat4 cameraView);
	glm::vec3 GetDirection();
	glm::mat4 GetViewMat(unsigned int i = 0);
	glm::mat4 GetProjMat(unsigned int i = 0);
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
	void SetCSMRatio(glm::vec2 ratio);
	void SetShadowEnlarge(float enlarge);
	void SetBias(glm::vec3 bias);
	void SetSoftDegree(float degree);
	void SetSoftShadow(bool softShadow);
	// Get spot light's members
	inline std::string GetName() const
	{
		return m_Name;
	};
	inline glm::vec3 GetEulerAngle() const
	{
		return m_EulerAngle;
	};
	inline glm::vec3 GetColor() const
	{
		return m_Color;
	};
	inline float GetAmbient() const
	{
		return m_ADS.x;
	};
	inline float GetDiffuse() const
	{
		return m_ADS.y;
	};
	inline float GetSpecular() const
	{
		return m_ADS.z;
	};
	inline glm::vec3 GetADS() const
	{
		return m_ADS;
	};
	inline float GetIntensity() const
	{
		return m_Intensity;
	};
	inline bool ShowCube() const
	{
		return m_ShowCube;
	};
	inline bool LightSwitch() const
	{
		return m_LightSwitch;
	};
	inline bool CastShadow() const
	{
		return m_CastShadow;
	};
	inline unsigned int GetShadowRes() const
	{
		return m_ShadowRes;
	};
	inline glm::vec2 GetCSMRatio() const
	{
		return m_CSMRatio;
	};
	inline float GetShadowEnlarge() const
	{
		return m_ShadowEnlarge;
	};
	inline glm::vec3 GetBias() const
	{
		return m_Bias;
	};
	inline bool SoftShadow() const
	{
		return m_SoftShadow;
	};
	inline float GetSoftDegree() const
	{
		return m_SoftDegree;
	};
	// Draw UI
	void DrawUI();
};