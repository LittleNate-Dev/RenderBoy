#include "Camera.h"

Camera::Camera()
{
	m_Type = true;
	m_FOV = 80.0f;
	m_WindowSize = glm::vec2(-1, -1);
	m_Plane = glm::vec2(0.1f, 500.0f);
	m_Position = glm::vec3(0.0f);
	m_EulerAngle = glm::vec3(0.0f);
	m_MoveSpeed = 1.0f;
	m_RotateSpeed = 1.0f;
}

Camera::~Camera()
{
}

glm::vec3 Camera::GetDirection(glm::vec3 direction)
{
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
	direction = glm::vec3(rotateMat * glm::vec4(direction, 0.0f));
	direction = glm::normalize(direction);
	return direction;
}

glm::mat4 Camera::GetViewMat()
{
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
	glm::vec4 axis = rotateMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	rotateMat = rbcore::GetRodrigue(glm::normalize(axis), m_EulerAngle.z) * rotateMat;
	glm::mat4 viewMat = translateMat * rotateMat;
	viewMat = glm::inverse(viewMat);
	return viewMat;
}

glm::mat4 Camera::GetProjMat()
{
	glm::mat4 projMat;
	// Perspective
	if (m_Type)
	{
		projMat = glm::perspective(glm::radians(m_FOV), (float)m_WindowSize.x / (float)m_WindowSize.y, m_Plane.x, m_Plane.y);
	}
	// Orthographic
	else
	{
		projMat = glm::ortho(-(float)m_WindowSize.x / 2, (float)m_WindowSize.x / 2, -(float)m_WindowSize.y / 2, (float)m_WindowSize.y / 2, -m_Plane.x, m_Plane.y);
	}
	return projMat;
}

void Camera::SetCameraType(bool type)
{
	m_Type = type;
}

void Camera::SetWindowSize()
{
	m_WindowSize = glm::vec2(rbcore::SETTINGS.width, rbcore::SETTINGS.height);
}

void Camera::SetWindowSize(unsigned int width, unsigned int height)
{
	m_WindowSize = glm::vec2(width, height);
}

void Camera::SetFOV(float fov)
{
	fov = fov <= 0.0f ? 1.0f : fov;
	m_FOV = fov;
}

void Camera::SetNearPlane(float nearPlane)
{
	nearPlane = nearPlane <= 0.0f ? 0.1f : nearPlane;
	nearPlane = nearPlane >= m_Plane.y ? m_Plane.y * 0.9f : nearPlane;
	m_Plane.x = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	farPlane = farPlane <= m_Plane.x ? m_Plane.x * 1.1f : farPlane;
	m_Plane.y = farPlane;
}

void Camera::SetPlane(float nearPlane, float farPlane)
{
	nearPlane = nearPlane <= 0.0f ? 0.1f : nearPlane;
	nearPlane = nearPlane >= farPlane ? farPlane * 0.9f : nearPlane;
	m_Plane = glm::vec2(nearPlane, farPlane);
}

void Camera::SetPlane(glm::vec2 plane)
{
	plane.x = plane.x <= 0.0f ? 0.1f : plane.x;
	plane.x = plane.x >= plane.y ? plane.y * 0.9f : plane.x;
	m_Plane = plane;
}

void Camera::SetPosition(float posX, float posY, float posZ)
{
	m_Position = glm::vec3(posX, posY, posZ);
}

void Camera::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void Camera::SetPitch(float pitch)
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

void Camera::SetYaw(float yaw)
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

void Camera::SetRoll(float roll)
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

void Camera::SetEulerAngle(float pitch, float yaw, float roll)
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

void Camera::SetEulerAngle(glm::vec3 eulerAngle)
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

void Camera::SetMoveSpeed(float speed)
{
	speed = speed < 0.0f ? 0.0f : speed;
	m_MoveSpeed = speed;
}

void Camera::SetRotateSpeed(float speed)
{
	speed = speed < 0.0f ? 0.0f : speed;
	m_RotateSpeed = speed;
}

bool Camera::GetCameraType()
{
	return m_Type;
}

glm::vec2 Camera::GetWindowSize()
{
	return m_WindowSize;
}

float Camera::GetFOV()
{
	return m_FOV;
}

float Camera::GetNearPlane()
{
	return m_Plane.x;
}

float Camera::GetFarPlane()
{
	return m_Plane.y;
}

glm::vec2 Camera::GetPlane()
{
	return m_Plane;
}

glm::vec3 Camera::GetPosition()
{
	return m_Position;
}

glm::vec3 Camera::GetEulerAngle()
{
	return m_EulerAngle;
}

float Camera::GetMoveSpeed()
{
	return m_MoveSpeed;
}

float Camera::GetRotateSpeed()
{
	return m_RotateSpeed;
}

void Camera::DrawUI()
{
	ImGui::LabelHighlighted("Position:");
	ImGui::Text("%.3f, %.3f, %.3f", m_Position.x, m_Position.y, m_Position.z);
	ImGui::LabelHighlighted("Rotation:");
	ImGui::Text("%.3f, %.3f, %.3f", m_EulerAngle.x, m_EulerAngle.y, m_EulerAngle.z);
	// Attributes
	if (ImGui::TreeNode("Attributes"))
	{
		ImGui::CenterAlignWidget("Near Plane", 120.0f);
		ImGui::LabelHighlighted("Near Plane");
		ImGui::PushItemWidth(120.0f);
		if (ImGui::InputFloat("##NearPlane", &m_Plane.x))
		{
			SetNearPlane(m_Plane.x);
		}
		ImGui::PopItemWidth();
		ImGui::CenterAlignWidget("Far Plane", 120.0f);
		ImGui::LabelHighlighted("Far Plane");
		ImGui::PushItemWidth(120.0f);
		if (ImGui::InputFloat("##FarPlane", &m_Plane.y))
		{
			SetFarPlane(m_Plane.y);
		}
		ImGui::PopItemWidth();
		ImGui::CenterAlignWidget("Move Speed", 120.0f);
		ImGui::LabelHighlighted("Move Speed");
		ImGui::PushItemWidth(120.0f);
		if (ImGui::InputFloat("##MoveSpeed", &m_MoveSpeed))
		{
			SetMoveSpeed(m_MoveSpeed);
		}
		ImGui::PopItemWidth();
		ImGui::CenterAlignWidget("Rotate Speed", 120.0f);
		ImGui::LabelHighlighted("Rotate Speed");
		ImGui::PushItemWidth(120.0f);
		if (ImGui::InputFloat("##RotateSpeed", &m_RotateSpeed))
		{
			SetRotateSpeed(m_RotateSpeed);
		}
		ImGui::PopItemWidth();
		ImGui::CenterAlignWidget("Reset");
		ImGui::TreePop();
	}
	// Position
	if (ImGui::TreeNode("Position"))
	{
		ImGui::PushItemWidth(80);
		ImGui::CenterAlignWidget(80);
		ImGui::InputFloat("Pos X", &m_Position.x);
		ImGui::CenterAlignWidget(80);
		ImGui::InputFloat("Pos Y", &m_Position.y);
		ImGui::CenterAlignWidget(80);
		ImGui::InputFloat("Pos Z", &m_Position.z);
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
			ImGui::PushItemWidth(280);
			ImGui::SliderFloat("Pitch", &m_EulerAngle.x, -360.0f, 360.0f);
			ImGui::SliderFloat("Yaw", &m_EulerAngle.y, -360.0f, 360.0f);
			ImGui::SliderFloat("Roll", &m_EulerAngle.z, -360.0f, 360.0f);
			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::PushItemWidth(80);
			ImGui::CenterAlignWidget(80);
			if (ImGui::InputFloat("Pitch", &m_EulerAngle.x))
			{
				SetPitch(m_EulerAngle.x);
			}
			ImGui::CenterAlignWidget(80);
			if (ImGui::InputFloat("Yaw", &m_EulerAngle.y))
			{
				SetYaw(m_EulerAngle.y);
			}
			ImGui::CenterAlignWidget(80);
			if (ImGui::InputFloat("Roll", &m_EulerAngle.z))
			{
				SetRoll(m_EulerAngle.z);
			}
			ImGui::PopItemWidth();
		}
		ImGui::TreePop();
	}
	ImGui::CenterAlignWidget("Reset");
	if (ImGui::Button("Reset"))
	{
		m_Plane = glm::vec2(0.1f, m_Plane.y);
		m_Position = glm::vec3(0.0f);
		m_EulerAngle = glm::vec3(0.0f);
	}
}