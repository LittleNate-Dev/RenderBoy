#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	m_EulerAngle = glm::vec3(0.0f);
	m_Color = glm::vec3(1.0f);
	m_ADS = glm::vec3(0.2f, 0.5f, 1.0f);
	m_Intensity = 1.0f;
	m_LightSwitch = true;
	m_ShowCube = true;
	m_CastShadow = true;
	m_ShadowRes = 1024;
	m_CSMRatio = glm::vec2(0.3f, 0.7f);
	for (unsigned int i = 0; i < 3; i++)
	{
		m_ProjMat[i] = glm::mat4(1.0f);
		m_ViewMat[i] = glm::mat4(1.0f);
	}
}

DirectionalLight::~DirectionalLight()
{
}

glm::vec3 DirectionalLight::GetDirection()
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
	direction = glm::vec3(rotateMat * glm::vec4(direction, 0.0f));
	direction = glm::normalize(direction);
	return direction;
}

glm::mat4 DirectionalLight::GetViewMat(unsigned int i)
{
	if (i > 2)
	{
		return glm::mat4(1.0f);
	}
	return m_ViewMat[i];
}

glm::mat4 DirectionalLight::GetProjMat(unsigned int i)
{
	if (i > 2)
	{
		return glm::mat4(1.0f);
	}
	return m_ProjMat[i];
}

glm::mat4 DirectionalLight::GetRotateMat()
{
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
	glm::vec4 axis = rotateMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	rotateMat = core::GetRodrigue(glm::normalize(axis), m_EulerAngle.z) * rotateMat;
	return rotateMat;
}

glm::mat4 DirectionalLight::GetModelMat()
{
	return GetRotateMat();
}

void DirectionalLight::SetName(std::string name)
{
	m_Name = name;
}

void DirectionalLight::SetPitch(float pitch)
{
	if (abs(pitch) > 360.0f)
	{
		if (pitch > 0.0f)
		{
			pitch = pitch - (float)((int)pitch / 360) * 360.0f;
		}
		else
		{
			pitch = pitch - (float)((int)abs(pitch) / 360) * -360.0f;
		}
	}
	m_EulerAngle.x = pitch;
}

void DirectionalLight::SetYaw(float yaw)
{
	if (abs(yaw) > 360.0f)
	{
		if (yaw > 0.0f)
		{
			yaw = yaw - (float)((int)yaw / 360) * 360.0f;
		}
		else
		{
			yaw = yaw - (float)((int)abs(yaw) / 360) * -360.0f;
		}
	}
	m_EulerAngle.y = yaw;
}

void DirectionalLight::SetRoll(float roll)
{
	if (abs(roll) > 360.0f)
	{
		if (roll > 0.0f)
		{
			roll = roll - (float)((int)roll / 360) * 360.0f;
		}
		else
		{
			roll = roll - (float)((int)abs(roll) / 360) * -360.0f;
		}
	}
	m_EulerAngle.z = roll;
}

void DirectionalLight::SetEulerAngle(float pitch, float yaw, float roll)
{
	if (abs(pitch) > 360.0f)
	{
		if (pitch > 0.0f)
		{
			pitch = pitch - (float)((int)pitch / 360) * 360.0f;
		}
		else
		{
			pitch = pitch - (float)((int)abs(pitch) / 360) * -360.0f;
		}
	}
	if (abs(yaw) > 360.0f)
	{
		if (yaw > 0.0f)
		{
			yaw = yaw - (float)((int)yaw / 360) * 360.0f;
		}
		else
		{
			yaw = yaw - (float)((int)abs(yaw) / 360) * -360.0f;
		}
	}
	if (abs(roll) > 360.0f)
	{
		if (roll > 0.0f)
		{
			roll = roll - (float)((int)roll / 360) * 360.0f;
		}
		else
		{
			roll = roll - (float)((int)abs(roll) / 360) * -360.0f;
		}
	}
	m_EulerAngle = glm::vec3(pitch, yaw, roll);
}

void DirectionalLight::SetEulerAngle(glm::vec3 eulerAngle)
{
	if (abs(eulerAngle.x) > 360.0f)
	{
		if (eulerAngle.x > 0.0f)
		{
			eulerAngle.x = eulerAngle.x - (float)((int)eulerAngle.x / 360) * 360.0f;
		}
		else
		{
			eulerAngle.x = eulerAngle.x - (float)((int)abs(eulerAngle.x) / 360) * -360.0f;
		}
	}
	if (abs(eulerAngle.y) > 360.0f)
	{
		if (eulerAngle.y > 0.0f)
		{
			eulerAngle.y = eulerAngle.y - (float)((int)eulerAngle.y / 360) * 360.0f;
		}
		else
		{
			eulerAngle.y = eulerAngle.y - (float)((int)abs(eulerAngle.y) / 360) * -360.0f;
		}
	}
	if (abs(eulerAngle.z) > 360.0f)
	{
		if (eulerAngle.z > 0.0f)
		{
			eulerAngle.z = eulerAngle.z - (float)((int)eulerAngle.z / 360) * 360.0f;
		}
		else
		{
			eulerAngle.z = eulerAngle.z - (float)((int)abs(eulerAngle.z) / 360) * -360.0f;
		}
	}
	m_EulerAngle = eulerAngle;
}

void DirectionalLight::SetColor(glm::vec3 color)
{
	color.x = color.x < 0.0f ? 0.0f : color.x;
	color.y = color.y < 0.0f ? 0.0f : color.y;
	color.z = color.z < 0.0f ? 0.0f : color.z;
	m_Color = color;
}

void DirectionalLight::SetAmbient(float ambient)
{
	ambient = ambient < 0.0f ? 0.0f : ambient;
	m_ADS.x = ambient;
}

void DirectionalLight::SetDiffuse(float diffuse)
{
	diffuse = diffuse < 0.0f ? 0.0f : diffuse;
	m_ADS.y = diffuse;
}

void DirectionalLight::SetSpecular(float specular)
{
	specular = specular < 0.0f ? 0.0f : specular;
	m_ADS.z = specular;
}

void DirectionalLight::SetADS(float ambient, float diffuse, float specular)
{
	ambient = ambient < 0.0f ? 0.0f : ambient;
	diffuse = diffuse < 0.0f ? 0.0f : diffuse;
	specular = specular < 0.0f ? 0.0f : specular;
	m_ADS = glm::vec3(ambient, diffuse, specular);
}

void DirectionalLight::SetADS(glm::vec3 ads)
{
	ads.x = ads.x < 0.0f ? 0.0f : ads.x;
	ads.y = ads.y < 0.0f ? 0.0f : ads.y;
	ads.z = ads.z < 0.0f ? 0.0f : ads.z;
	m_ADS = ads;
}

void DirectionalLight::SetIntensity(float intensity)
{
	intensity = intensity < 0.0f ? 0.0f : intensity;
	m_Intensity = intensity;
}

void DirectionalLight::SetShowCube(bool drawCube)
{
	m_ShowCube = drawCube;
}

void DirectionalLight::SetLightSwitch(bool lightSwitch)
{
	m_LightSwitch = lightSwitch;
}

void DirectionalLight::SetCastShadow(bool castShadow)
{
	m_CastShadow = castShadow;
}

void DirectionalLight::SetShadowRes(unsigned int res)
{
	if (res <= 1024)
	{
		res = 1024;
	}
	else if (res <= 2048)
	{
		res = 2048;
	}
	else if (res <= 3072)
	{
		res = 3072;
	}
	else
	{
		res = 4096;
	}
}

void DirectionalLight::SetCSMRatio(glm::vec2 ratio)
{
	if (ratio.x <= 0.0f || ratio.x >= 1.0f)
	{
		return;
	}
	if (ratio.y <= 0.0f || ratio.y >= 1.0f)
	{
		return;
	}
	if (ratio.x >= ratio.y)
	{
		return;
	}
	m_CSMRatio = ratio;
}

void DirectionalLight::DrawUI()
{
	// Settings
	if (ImGui::TreeNode("Settings"))
	{
		ImGui::CenterAlignWidget("Switch");
		ImGui::LabelHighlighted("Switch");
		ImGui::Checkbox("##Switch", &m_LightSwitch);
		if (m_LightSwitch)
		{
			ImGui::CenterAlignWidget("Show Light Cube");
			ImGui::LabelHighlighted("Show Light Cube");
			ImGui::Checkbox("##ShowLightCube", &m_ShowCube);
			ImGui::CenterAlignWidget("Cast Shadow");
			ImGui::LabelHighlighted("Cast Shadow");
			ImGui::Checkbox("##CastShadow", &m_CastShadow);
		}
		ImGui::TreePop();
	}
	if (m_LightSwitch)
	{
		// Attributes
		if (ImGui::TreeNode("Attributes"))
		{
			// Basic attributes
			if (ImGui::TreeNode("Basic"))
			{
				ImGui::CenterAlignWidget("Color", 200.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Color");
				ImGui::PushItemWidth(200.0f * core::GetWidgetWidthCoefficient());
				ImGui::ColorEdit3("##DirColor", &m_Color[0]);
				ImGui::PopItemWidth();
				ImGui::CenterAlignWidget("Intensity", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Intensity");
				ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
				if (ImGui::InputFloat("##DirIntensity", &m_Intensity, 0.0f, 0.0f, "%.6f"))
				{
					SetIntensity(m_Intensity);
				}
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			// Shadow resolution
			if (m_CastShadow && ImGui::TreeNode("Shadow"))
			{
				ImGui::CenterAlignWidget("Shadow Resolution", 60.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Shadow Resolution");
				ImGui::PushItemWidth(60.0f * core::GetWidgetWidthCoefficient());
				const char* shadowResOps[] = {
					"1X",
					"2X",
					"3X",
					"4X"
				};
				static int currentRes = 0;
				if (ImGui::Combo("##ShadowRes", &currentRes, shadowResOps, IM_ARRAYSIZE(shadowResOps)))
				{
					switch (currentRes)
					{
					case 0:
						m_ShadowRes = 1024;
						break;
					case 1:
						m_ShadowRes = 2048;
						break;
					case 2:
						m_ShadowRes = 3072;
						break;
					case 3:
						m_ShadowRes = 4096;
						break;
					default:
						m_ShadowRes = 1024;
						break;
					}
				}
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			// Advanced settings
			if (ImGui::TreeNode("Advanced"))
			{
				ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
				ImGui::CenterAlignWidget("Ambient", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Ambient");
				if (ImGui::InputFloat("##Ambient", &m_ADS.x, 0.0f, 0.0f, "%.6f"))
				{
					SetAmbient(m_ADS.x);
				}
				ImGui::CenterAlignWidget("Diffuse", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Diffuse");
				if (ImGui::InputFloat("##Diffuse", &m_ADS.y, 0.0f, 0.0f, "%.6f"))
				{
					SetDiffuse(m_ADS.y);
				}
				ImGui::CenterAlignWidget("Specular", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Specular");
				if (ImGui::InputFloat("##Specular", &m_ADS.z, 0.0f, 0.0f, "%.6f"))
				{
					SetSpecular(m_ADS.z);
				}
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// Rotation
		static bool slideRotate = true;
		if (ImGui::TreeNode("Rotation"))
		{
			ImGui::Checkbox("Slider", &slideRotate);
			if (slideRotate)
			{
				ImGui::PushItemWidth(280.0f * core::GetWidgetWidthCoefficient());
				ImGui::CenterAlignWidget("Pitch", 280.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Pitch");
				ImGui::SliderFloat("##Pitch", &m_EulerAngle.x, -360.0f, 360.0f);
				ImGui::CenterAlignWidget("Yaw", 280.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Yaw");
				ImGui::SliderFloat("##Yaw", &m_EulerAngle.y, -360.0f, 360.0f);
				ImGui::PopItemWidth();
			}
			else
			{
				ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
				ImGui::CenterAlignWidget("Pitch", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Pitch");
				if (ImGui::InputFloat("##Pitch", &m_EulerAngle.x))
				{
					SetPitch(m_EulerAngle.x);
				}
				ImGui::CenterAlignWidget("Yaw", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Yaw");
				if (ImGui::InputFloat("##Yaw", &m_EulerAngle.y))
				{
					SetYaw(m_EulerAngle.y);
				}
				ImGui::PopItemWidth();
			}
			ImGui::TreePop();
		}
	}
	// Reset
	ImGui::CenterAlignWidget("Reset");
	if (ImGui::Button("Reset"))
	{
		m_EulerAngle = glm::vec3(0.0f);
		m_Color = glm::vec3(1.0f);
		m_ADS = glm::vec3(0.2f, 0.5f, 1.0f);
		m_Intensity = 1.0f;
		m_LightSwitch = true;
		m_ShowCube = true;
		m_CastShadow = true;
		m_ShadowRes = 1024;
	}
}