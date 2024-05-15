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
	inline std::string GetName() const
	{
		return m_Name;
	};
	inline glm::vec3 GetPosition() const
	{
		return m_Position;
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
	inline float GetRange() const
	{
		return m_Range;
	};
	inline float GetIntensity() const
	{
		return m_Intensity;
	};
	inline float GetConstant() const
	{
		return m_CLQ.x;
	};
	inline float GetLinear() const
	{
		return m_CLQ.y;
	};
	inline float GetQuadratic() const
	{
		return m_CLQ.z;
	};
	inline glm::vec3 GetCLQ() const
	{
		return m_CLQ;
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
	// Draw UI
	void DrawUI();
};