/*
* Class:
*	AreaLight
* Description:
*	This class represents area light
*/

#pragma once

#include "core/Core.h"
#include "data/Data.h"

enum AL_Type
{
	RECTANGLE = 0,
	CYLINDER = 1,
	DISK = 2,
	SPHERE = 3
};

class AreaLight
{
private:
	std::string m_Name;
	glm::mat4 m_ModelMat;
	glm::vec3 m_Position;
	glm::vec3 m_EulerAngle;
	glm::vec3 m_Scale;
	glm::vec3 m_Color;
	glm::vec3 m_CLQ; // Constant, Linear and Quadratic
	glm::vec3 m_ADS; // Ambient, Diffuse, Specular
	AL_Type m_Type;
	glm::vec3 m_Points[4];
	bool m_TwoSided;
	float m_Intensity;
	// Turn on/off light
	bool m_LightSwitch;
	// Show light cube
	bool m_ShowCube;

	void UpdatePoints();
	void UpdatePointsRect();
	void UpdatePointsDisk();
	void UpdatePointsCylinder();
	void UpdatePointsSphere();

public:
	AreaLight();
	~AreaLight();

	void UpdateModelMat();
	// Set area light's attributes
	void SetName(std::string name);
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 position);
	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetRoll(float roll);
	void SetEulerAngle(float pitch, float yaw, float roll);
	void SetEulerAngle(glm::vec3 eulerAngle);
	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 scale);
	void SetType(AL_Type type);
	inline void SetTwoSided(bool twoSide)
	{
		m_TwoSided = twoSide;
	};
	void SetColor(glm::vec3 color);
	void SetIntensity(float intensity);
	void SetLightSwitch(bool lightSwitch);
	void SetShowCube(bool showCube);
	// Get area light's attributes
	inline std::string GetName() const
	{
		return m_Name;
	};
	glm::mat4 GetTranslateMat();
	glm::mat4 GetRotateMat();
	glm::mat4 GetScaleMat();
	inline glm::mat4 GetModelMat() const
	{
		return m_ModelMat;
	};
	inline glm::vec3 GetPosition() const
	{
		return m_Position;
	};
	inline glm::vec3 GetEulerAngle() const
	{
		return m_EulerAngle;
	};
	inline glm::vec3 GetScale() const
	{
		return m_Scale;
	};
	inline glm::vec3 GetColor() const 
	{
		return m_Color;
	};
	inline float GetIntensity() const
	{
		return m_Intensity;
	};
	inline AL_Type GetLightType() const
	{
		return m_Type;
	};
	inline glm::vec3 GetPoints(unsigned int i) const
	{
		return m_Points[i];
	};
	inline bool TwoSided() const
	{
		return m_TwoSided;
	};
	inline bool LightSwitch() const
	{
		return m_LightSwitch;
	};
	inline bool ShowCube() const
	{
		return m_ShowCube;
	};
	// Draw UI
	void DrawUI();
};

