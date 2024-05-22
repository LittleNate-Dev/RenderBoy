#include "PointLight.h"

PointLight::PointLight()
{
	m_Position = glm::vec3(0.0f);
	m_Color = glm::vec3(1.0f);
	m_ADS = glm::vec3(0.2f, 0.5f, 1.0f);
	m_Range = 10.0f;
	m_Intensity = 1.0f;
	m_CLQ = glm::vec3(1.0f, 0.35f, 0.44f);
	m_LightSwitch = true;
	m_ShowCube = false;
	m_CastShadow = true;
	m_ShadowRes = 1024;
}

PointLight::~PointLight()
{
}

glm::mat4 PointLight::GetViewMat(unsigned int face)
{
	glm::mat4 rotateMat = glm::mat4(1.0f);
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), m_Position);
	switch (face)
	{
	case 0: // right
		rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 0, 1));
		break;
	case 1: // left
		rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 0, 1));
		break;
	case 2: // top
		rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));
		break;
	case 3: // bottom
		rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0));
		break;
	case 4: // front
		rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 0, 1));
		break;
	case 5: // back
		rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(360.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 0, 1));
		break;
	}
	glm::mat4 viewMat = translateMat * rotateMat;
	viewMat = glm::inverse(viewMat);
	return viewMat;
}

glm::mat4 PointLight::GetProjMat()
{
	glm::mat4 projMat = glm::perspective(glm::radians(90.0f), 1.0f, 0.05f, m_Intensity);
	return projMat;
}

glm::mat4 PointLight::GetTranslateMat()
{
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), m_Position);
	return translateMat;
}

glm::mat4 PointLight::GetModelMat()
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), m_Position);
	return modelMat;
}

void PointLight::SetName(std::string name)
{
	m_Name = name;
}

void PointLight::SetPosition(float posX, float posY, float posZ)
{
	m_Position = glm::vec3(posX, posY, posZ);
}

void PointLight::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void PointLight::SetColor(glm::vec3 color)
{
	color.x = color.x < 0.0f ? 0.0f : color.x;
	color.y = color.y < 0.0f ? 0.0f : color.y;
	color.z = color.z < 0.0f ? 0.0f : color.z;
	m_Color = color;
}

void PointLight::SetAmbient(float ambient)
{
	ambient = ambient < 0.0f ? 0.0f : ambient;
	m_ADS.x = ambient;
}

void PointLight::SetDiffuse(float diffuse)
{
	diffuse = diffuse < 0.0f ? 0.0f : diffuse;
	m_ADS.y = diffuse;
}

void PointLight::SetSpecular(float specular)
{
	specular = specular < 0.0f ? 0.0f : specular;
	m_ADS.z = specular;
}

void PointLight::SetADS(float ambient, float diffuse, float specular)
{
	ambient = ambient < 0.0f ? 0.0f : ambient;
	diffuse = diffuse < 0.0f ? 0.0f : diffuse;
	specular = specular < 0.0f ? 0.0f : specular;
	m_ADS = glm::vec3(ambient, diffuse, specular);
}

void PointLight::SetADS(glm::vec3 ads)
{
	ads.x = ads.x < 0.0f ? 0.0f : ads.x;
	ads.y = ads.y < 0.0f ? 0.0f : ads.y;
	ads.z = ads.z < 0.0f ? 0.0f : ads.z;
	m_ADS = ads;
}

void PointLight::SetRange(float range)
{
	range = range < 0.0f ? 0.0f : range;
	m_Range = range;
	m_CLQ = core::GetAttenuationValues(m_Range);
}

void PointLight::SetIntensity(float intensity)
{
	intensity = intensity < 0.0f ? 0.0f : intensity;
	m_Intensity = intensity;
}

void PointLight::SetConstant(float constant)
{
	m_CLQ.x = constant;
}

void PointLight::SetLinear(float linear)
{
	m_CLQ.y = linear;
}

void PointLight::SetQuadratic(float quadratic)
{
	m_CLQ.z = quadratic;
}

void PointLight::SetCLQ(float constant, float linear, float quadratic)
{
	m_CLQ = glm::vec3(constant, linear, quadratic);
}

void PointLight::SetCLQ(glm::vec3 clq)
{
	m_CLQ = clq;
}

void PointLight::SetShowCube(bool drawCube)
{
	m_ShowCube = drawCube;
}

void PointLight::SetLightSwitch(bool lightSwitch)
{
	m_LightSwitch = lightSwitch;
}

void PointLight::SetCastShadow(bool castShadow)
{
	m_CastShadow = castShadow;
}

void PointLight::SetShadowRes(unsigned int res)
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

void PointLight::DrawUI()
{
	if (m_LightSwitch)
	{
		// Attributes
		if (ImGui::TreeNode("Attributes"))
		{
			ImGui::CenterAlignWidget("Color", 200.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Color");
			ImGui::PushItemWidth(200.0f * core::GetWidgetWidthCoefficient());
			ImGui::ColorEdit3("##PointColor", &m_Color[0]);
			ImGui::PopItemWidth();			
			ImGui::CenterAlignWidget("Range", 120.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Range");
			ImGui::PushItemWidth(120.0f * core::GetWidgetWidthCoefficient());
			if (ImGui::InputFloat("##PointRange", &m_Range))
			{
				SetRange(m_Range);
			}
			ImGui::PopItemWidth();
			ImGui::CenterAlignWidget("Intensity", 80.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Intensity");
			ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
			if (ImGui::InputFloat("##PointIntensity", &m_Intensity, 0.0f, 0.0f, "%.6f"))
			{
				SetIntensity(m_Intensity);
			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		// Position
		if (ImGui::TreeNode("Position"))
		{
			ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
			ImGui::CenterAlignWidget(80.0f * core::GetWidgetWidthCoefficient());
			ImGui::InputFloat("Pos X", &m_Position.x);
			ImGui::CenterAlignWidget(80.0f * core::GetWidgetWidthCoefficient());
			ImGui::InputFloat("Pos Y", &m_Position.y);
			ImGui::CenterAlignWidget(80.0f * core::GetWidgetWidthCoefficient());
			ImGui::InputFloat("Pos Z", &m_Position.z);
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
	}
	// Settings
	if (ImGui::TreeNode("Settings"))
	{
		ImGui::Checkbox("Switch", &m_LightSwitch);
		if (m_LightSwitch)
		{
			ImGui::Checkbox("Show Light Cube", &m_ShowCube);
			ImGui::Checkbox("Cast Shadow", &m_CastShadow);
		}
		// Shadow resolution
		if (m_CastShadow && m_LightSwitch)
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
		}
		ImGui::TreePop();
	}
	// Advanced settings
	if (m_LightSwitch)
	{
		// Advanced settings
		if (ImGui::TreeNode("Advanced settings"))
		{
			ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
			ImGui::CenterAlignWidget(80.0f * core::GetWidgetWidthCoefficient());
			if (ImGui::InputFloat("Ambient", &m_ADS.x, 0.0f, 0.0f, "%.6f"))
			{
				SetAmbient(m_ADS.x);
			}
			ImGui::CenterAlignWidget(80.0f * core::GetWidgetWidthCoefficient());
			if (ImGui::InputFloat("Diffuse", &m_ADS.y, 0.0f, 0.0f, "%.6f"))
			{
				SetDiffuse(m_ADS.y);
			}
			ImGui::CenterAlignWidget(80.0f * core::GetWidgetWidthCoefficient());
			if (ImGui::InputFloat("Specular", &m_ADS.z, 0.0f, 0.0f, "%.6f"))
			{
				SetSpecular(m_ADS.z);
			}
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(100.0f * core::GetWidgetWidthCoefficient());
			ImGui::CenterAlignWidget(100.0f * core::GetWidgetWidthCoefficient());
			ImGui::InputFloat("Constant", &m_CLQ.x, 0.0f, 0.0f, "%.8f");
			ImGui::CenterAlignWidget(100.0f * core::GetWidgetWidthCoefficient());
			ImGui::InputFloat("Linear", &m_CLQ.y, 0.0f, 0.0f, "%.8f");
			ImGui::CenterAlignWidget(100.0f * core::GetWidgetWidthCoefficient());
			ImGui::InputFloat("Quadratic", &m_CLQ.z, 0.0f, 0.0f, "%.8f");
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
	}
	// Reset
	ImGui::CenterAlignWidget("Reset");
	if (ImGui::Button("Reset"))
	{
		m_Position = glm::vec3(0.0f);
		m_Color = glm::vec3(1.0f);
		m_ADS = glm::vec3(0.2f, 0.5f, 1.0f);
		m_Range = 10.0f;
		m_Intensity = 1.0f;
		m_CLQ = glm::vec3(1.0f, 0.35f, 0.44f);
		m_LightSwitch = true;
		m_ShowCube = false;
		m_CastShadow = true;
		m_ShadowRes = 1024;
	}
}