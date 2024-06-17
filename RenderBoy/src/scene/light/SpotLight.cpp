#include "SpotLight.h"

SpotLight::SpotLight()
{
	m_Position = glm::vec3(0.0f);
	m_EulerAngle = glm::vec3(0.0f);
	m_Color = glm::vec3(1.0f);
	m_Direction = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Angle = 90.0f;
	m_DimAngle = 2.0f;
	m_ADS = glm::vec3(0.2f, 0.5f, 1.0f);
	m_Range = 10.0f;
	m_Intensity = 1.0f;
	m_CLQ = glm::vec3(1.0f, 0.35f, 0.44f);
	m_LightSwitch = true;
	m_ShowCube = true;
	m_CastShadow = true;
	m_ShadowRes = 1024;
	m_SoftShadow = true;
	m_SoftDegree = 2;
	m_Bias = glm::vec2(0.0f);
	m_ProjMat = glm::mat4(1.0f);
	m_ViewMat = glm::mat4(1.0f);
	m_ModelMat = glm::mat4(1.0f);
	UpdateDirection();
	UpdateProjMat();
	UpdateViewMat();
	UpdateModelMat();
}

SpotLight::~SpotLight()
{
}

void SpotLight::UpdateProjMat()
{
	m_ProjMat = glm::perspective(glm::radians(m_Angle + m_DimAngle), 1.0f, 0.01f, GetFarPlane());
}

void SpotLight::UpdateViewMat()
{
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x + 180.0f), glm::vec3(1, 0, 0));
	glm::vec4 axis = rotateMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	rotateMat = core::GetRodrigue(glm::normalize(axis), m_EulerAngle.z + 180.0f) * rotateMat;
	m_ViewMat = glm::inverse(translateMat * rotateMat);
}

void SpotLight::UpdateModelMat()
{
	glm::mat4 modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(sqrt(2.0f) * sin(glm::radians(m_Angle / 2.0f)), sqrt(2.0f) * sin(glm::radians(m_Angle / 2.0f)), sqrt(2.0f) * cos(glm::radians(m_Angle / 2.0f))));
	m_ModelMat = GetTranslateMat() * GetRotateMat() * modelMat;
}

void SpotLight::UpdateDirection()
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
	direction = glm::vec3(rotateMat * glm::vec4(direction, 0.0f));
	m_Direction = glm::normalize(direction);
}

glm::vec3 SpotLight::GetDirection()
{
	return m_Direction;
}

glm::mat4 SpotLight::GetViewMat()
{
	return m_ViewMat;
}

glm::mat4 SpotLight::GetProjMat()
{
	
	return m_ProjMat;
}

glm::mat4 SpotLight::GetTranslateMat()
{
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), m_Position);
	return translateMat;
}

glm::mat4 SpotLight::GetRotateMat()
{
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
	glm::vec4 axis = rotateMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	rotateMat = core::GetRodrigue(glm::normalize(axis), m_EulerAngle.z) * rotateMat;
	return rotateMat;
}

glm::mat4 SpotLight::GetModelMat()
{
	return m_ModelMat;
}

void SpotLight::SetName(std::string name)
{
	m_Name = name;
}

void SpotLight::SetPosition(float posX, float posY, float posZ)
{
	m_Position = glm::vec3(posX, posY, posZ);
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetPosition(glm::vec3 position)
{
	m_Position = position;
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetPitch(float pitch)
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
	UpdateDirection();
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetYaw(float yaw)
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
	UpdateDirection();
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetRoll(float roll)
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
	UpdateDirection();
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetEulerAngle(float pitch, float yaw, float roll)
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
	UpdateDirection();
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetEulerAngle(glm::vec3 eulerAngle)
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
	UpdateDirection();
	UpdateViewMat();
	UpdateModelMat();
}

void SpotLight::SetColor(glm::vec3 color)
{
	color.x = color.x < 0.0f ? 0.0f : color.x;
	color.y = color.y < 0.0f ? 0.0f : color.y;
	color.z = color.z < 0.0f ? 0.0f : color.z;
	m_Color = color;
}

void SpotLight::SetAngle(float angle)
{
	angle = angle < 0.0f ? 0.0f : angle;
	angle = angle + m_DimAngle * 2.0f > 180.0f ? 180.0f - m_DimAngle * 2.0f : angle;
	m_Angle = angle;
	UpdateProjMat();
	UpdateModelMat();
}

void SpotLight::SetDimAngle(float dimAngle)
{
	dimAngle = dimAngle < 0.0f ? 0.0f : dimAngle;
	dimAngle = m_Angle + dimAngle * 2.0f > 180.0f ? (180.0f - m_Angle) * 0.5f : dimAngle;
	m_DimAngle = dimAngle;
	UpdateProjMat();
	UpdateModelMat();
}

void SpotLight::SetAmbient(float ambient)
{
	ambient = ambient < 0.0f ? 0.0f : ambient;
	m_ADS.x = ambient;
}

void SpotLight::SetDiffuse(float diffuse)
{
	diffuse = diffuse < 0.0f ? 0.0f : diffuse;
	m_ADS.y = diffuse;
}

void SpotLight::SetSpecular(float specular)
{
	specular = specular < 0.0f ? 0.0f : specular;
	m_ADS.z = specular;
}

void SpotLight::SetADS(float ambient, float diffuse, float specular)
{
	ambient = ambient < 0.0f ? 0.0f : ambient;
	diffuse = diffuse < 0.0f ? 0.0f : diffuse;
	specular = specular < 0.0f ? 0.0f : specular;
	m_ADS = glm::vec3(ambient, diffuse, specular);
}

void SpotLight::SetADS(glm::vec3 ads)
{
	ads.x = ads.x < 0.0f ? 0.0f : ads.x;
	ads.y = ads.y < 0.0f ? 0.0f : ads.y;
	ads.z = ads.z < 0.0f ? 0.0f : ads.z;
	m_ADS = ads;
}

void SpotLight::SetRange(float range)
{
	range = range < 0.0f ? 0.0f : range;
	m_Range = range;
	m_CLQ = core::GetAttenuationValues(m_Range);
	UpdateProjMat();
}

void SpotLight::SetIntensity(float intensity)
{
	intensity = intensity < 0.0f ? 0.0f : intensity;
	m_Intensity = intensity;
}

void SpotLight::SetConstant(float constant)
{
	m_CLQ.x = constant;
}

void SpotLight::SetLinear(float linear)
{
	m_CLQ.y = linear;
}

void SpotLight::SetQuadratic(float quadratic)
{
	m_CLQ.z = quadratic;
}

void SpotLight::SetCLQ(float constant, float linear, float quadratic)
{
	m_CLQ = glm::vec3(constant, linear, quadratic);
}

void SpotLight::SetCLQ(glm::vec3 clq)
{
	m_CLQ = clq;
}

void SpotLight::SetBias(glm::vec2 bias)
{
	m_Bias = bias;
}

void SpotLight::SetShowCube(bool showCube)
{
	m_ShowCube = showCube;
}

void SpotLight::SetLightSwitch(bool lightSwitch)
{
	m_LightSwitch = lightSwitch;
}

void SpotLight::SetCastShadow(bool castShadow)
{
	m_CastShadow = castShadow;
}

void SpotLight::SetShadowRes(unsigned int res)
{
	res = res == 0 ? m_ShadowRes : res;
	m_ShadowRes = res;
	((Data*)core::SCENE_DATA)->SetShadowRes(m_Name, m_ShadowRes, m_ShadowRes, SPOT_LIGHT);
}

void SpotLight::SetSoftShadow(bool softShadow)
{
	m_SoftShadow = softShadow;
}

void SpotLight::SetSoftDegree(unsigned int degree)
{
	m_SoftDegree = degree;
}

void SpotLight::DrawUI()
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
				ImGui::ColorEdit3("##SpotColor", &m_Color[0]);
				ImGui::PopItemWidth();
				static bool slider = true;
				ImGui::Checkbox("Slider", &slider);
				if (slider)
				{
					ImGui::CenterAlignWidget("Angle", 150.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Angle");
					ImGui::PushItemWidth(150.0f * core::GetWidgetWidthCoefficient());
					if (ImGui::SliderFloat("##Angle", &m_Angle, 0.0f, 180.0f - m_DimAngle * 2.0f))
					{
						UpdateProjMat();
						UpdateModelMat();
					}
					ImGui::CenterAlignWidget("Dim Angle", 150.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Dim Angle");
					if (ImGui::SliderFloat("##DimAngle", &m_DimAngle, 0.0f, (180.0f - m_Angle) * 0.5f))
					{
						UpdateProjMat();
						UpdateModelMat();
					}
					ImGui::PopItemWidth();
				}
				else
				{
					ImGui::CenterAlignWidget("Angle", 80.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Angle");
					ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
					if (ImGui::InputFloat("##Angle", &m_Angle))
					{
						SetAngle(m_Angle);
					}
					ImGui::CenterAlignWidget("Dim Angle", 80.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Dim Angle");
					if (ImGui::InputFloat("##DimAngle", &m_DimAngle))
					{
						SetDimAngle(m_DimAngle);
					}
					ImGui::PopItemWidth();
				}
				ImGui::CenterAlignWidget("Range", 120.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Range");
				ImGui::PushItemWidth(120.0f * core::GetWidgetWidthCoefficient());
				if (ImGui::InputFloat("##SpotRange", &m_Range))
				{
					SetRange(m_Range);
				}
				ImGui::PopItemWidth();
				ImGui::CenterAlignWidget("Intensity", 80.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Intensity");
				ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
				if (ImGui::InputFloat("##SpotIntensity", &m_Intensity, 0.0f, 0.0f, "%.6f"))
				{
					SetIntensity(m_Intensity);
				}
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			// Shadow
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
				static int currentRes;
				switch (m_ShadowRes)
				{
				case 1024:
					currentRes = 0;
					break;
				case 2048:
					currentRes = 1;
					break;
				case 3072:
					currentRes = 2;
					break;
				case 4096:
					currentRes = 3;
					break;
				}
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
					((Data*)core::SCENE_DATA)->SetShadowRes(m_Name, m_ShadowRes, m_ShadowRes, SPOT_LIGHT);
				}
				ImGui::PopItemWidth();
				ImGui::PushItemWidth(100.0f * core::GetWidgetWidthCoefficient());
				ImGui::CenterAlignWidget("Min Bias",100.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Min Bias");
				ImGui::InputFloat("##MinBias", &m_Bias.x, 0.0f, 0.0f, "%.8f");
				ImGui::CenterAlignWidget("Max Bias", 100.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Max Bias");
				ImGui::InputFloat("##MaxBias", &m_Bias.y, 0.0f, 0.0f, "%.8f");
				ImGui::PopItemWidth();
				ImGui::CenterAlignWidget("Soft Shadow");
				ImGui::LabelHighlighted("Soft Shadow");
				ImGui::Checkbox("##SoftShadow", &m_SoftShadow);
				if (m_SoftShadow)
				{
					ImGui::PushItemWidth(90.0f * core::GetWidgetWidthCoefficient());
					ImGui::CenterAlignWidget("Degree", 90.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Degree");
					if (ImGui::InputInt("##Degree", &m_SoftDegree))
					{
						SetSoftDegree(m_SoftDegree);
					}
					ImGui::PopItemWidth();
				}
				ImGui::TreePop();
			}
			// Advanced attributes
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
				ImGui::PushItemWidth(100.0f * core::GetWidgetWidthCoefficient());
				ImGui::CenterAlignWidget("Constant", 100.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Constant");
				ImGui::InputFloat("##Constant", &m_CLQ.x, 0.0f, 0.0f, "%.8f");
				ImGui::CenterAlignWidget("Linear", 100.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Linear");
				ImGui::InputFloat("##Linear", &m_CLQ.y, 0.0f, 0.0f, "%.8f");
				ImGui::CenterAlignWidget("Quadratic", 100.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Quadratic");
				ImGui::InputFloat("##Quadratic", &m_CLQ.z, 0.0f, 0.0f, "%.8f");
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		// Position
		if (ImGui::TreeNode("Position"))
		{
			ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
			ImGui::CenterAlignWidget("X", 80.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("X");
			if (ImGui::InputFloat("##X", &m_Position.x))
			{
				UpdateViewMat();
				UpdateModelMat();
			}
			ImGui::CenterAlignWidget("Y", 80.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Y");
			if (ImGui::InputFloat("##Y", &m_Position.y))
			{
				UpdateViewMat();
				UpdateModelMat();
			}
			ImGui::CenterAlignWidget("Z", 80.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Z");
			if (ImGui::InputFloat("##Z", &m_Position.z))
			{
				UpdateViewMat();
				UpdateModelMat();
			}
			ImGui::PopItemWidth();
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
				if (ImGui::SliderFloat("##Pitch", &m_EulerAngle.x, -360.0f, 360.0f))
				{
					UpdateDirection();
					UpdateViewMat();
					UpdateModelMat();
				}
				ImGui::CenterAlignWidget("Yaw", 280.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Yaw");
				if (ImGui::SliderFloat("##Yaw", &m_EulerAngle.y, -360.0f, 360.0f))
				{
					UpdateDirection();
					UpdateViewMat();
					UpdateModelMat();
				}
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
		m_Position = glm::vec3(0.0f);
		m_EulerAngle = glm::vec3(0.0f);
		m_Color = glm::vec3(1.0f);
		m_Angle = 90.0f;
		m_DimAngle = 2.0f;
		m_ADS = glm::vec3(0.2f, 0.5f, 1.0f);
		m_Range = 10.0f;
		m_Intensity = 1.0f;
		m_CLQ = glm::vec3(1.0f, 0.35f, 0.44f);
		m_LightSwitch = true;
		m_ShowCube = true;
		m_CastShadow = true;
		m_ShadowRes = 1024;
		m_SoftShadow = true;
		m_SoftDegree = 2;
		m_Bias = glm::vec2(0.0f);
		UpdateProjMat();
		UpdateViewMat();
		UpdateModelMat();
	}
}