#include "Scene.h"

Scene::Scene()
{
	m_FilePath = "";
	m_Name = "Default";
}

Scene::~Scene()
{
}

void Scene::Init()
{
	m_Data.Init();
}

void Scene::Reset()
{
	m_Name = "Default";
	m_FilePath = "";
	m_Camera.SetPosition(glm::vec3(0.0f));
	m_Camera.SetEulerAngle(glm::vec3(0.0f));
	m_Data.Reset();
	std::vector<std::string>().swap(m_ModelList);
	m_Models.clear();
	std::vector<std::string>().swap(m_PointLightList);
	m_PointLights.clear();
	std::vector<std::string>().swap(m_SpotLightList);
	m_SpotLights.clear();
	std::vector<std::string>().swap(m_DirLightList);
	m_DirLights.clear();
	rbcore::currentModelScene = nullptr;
	rbcore::currentModelInfo = nullptr;
	rbcore::currentPointLight = nullptr;
	rbcore::currentSpotLight = nullptr;
	rbcore::currentDirLight = nullptr;
}

bool Scene::Reset(std::string filepath)
{
	std::string fileType = rbcore::GetFileFormat(filepath);
	if (fileType == "scene")
	{
		if (LoadScene(filepath))
		{
			return true;
		}
	}
	else if (rbcore::CheckFileFormat(fileType))
	{
		Reset();
		if (AddModel(filepath))
		{
			return true;
		}
	}
	return false;
}

bool Scene::LoadScene(std::string filepath)
{
	Reset();
	std::ifstream stream(filepath);
	std::string line;
	// If setting file doesn't exist, creat a new one
	if (!stream)
	{
		spdlog::warn("Loading " + rbcore::GetFileName(filepath) + " failed!");
		rbcore::ShowWarningMsg("Loading " + rbcore::GetFileName(filepath) + " failed!");
		return false;
	}
	else
	{
		m_FilePath = filepath;
		std::vector<std::string> values;
		std::string model, light;
		while (getline(stream, line))
		{
			// Load scene info
			if (line.find("#SCENE_NAME") != std::string::npos)
			{
				values = rbcore::GetSceneValue(line);
				m_Name = values[0];
			}
			// Load Camera
			{
				if (line.find("#CAMERA_POSITION") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Camera.SetPosition(pos);
				}
				if (line.find("#CAMERA_EULERANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Camera.SetEulerAngle(euler);
				}
			}
			// Load models
			{
				if (line.find("#MODEL_NAME") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					model = values[0];
				}
				if (line.find("#" + model + "_FILEPATH") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					std::string modelPath;
					for (unsigned int i = 0; i < values.size(); i++)
					{
						modelPath += values[i];
						if (i != values.size() - 1)
						{
							modelPath += " ";
						}
					}
					if (!AddModel(model, modelPath))
					{
						return false;
						break;
					}
				}
				if (line.find("#" + model + "_POSITION") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Models[model].SetPosition(pos);
				}
				if (line.find("#" + model + "_SCALE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 scale = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Models[model].SetScale(scale);
				}
				if (line.find("#" + model + "_EULERANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Models[model].SetEulerAngle(euler);
				}
				if (line.find("#" + model + "_INSTANCE_NUM") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_Models[model].SetInstance(std::atoi(values[0].c_str()));
				}
				if (line.find("#" + model + "_CURRENT") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_Models[model].SetCurrent(std::atoi(values[0].c_str()));
				}
				if (line.find("#" + model + "_INSTANCE_POSITION") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					int current = std::atoi(values[0].c_str());
					glm::vec3 pos = glm::vec3((float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()), (float)std::atof(values[3].c_str()));
					m_Models[model].SetPosition(pos, current);
				}
				if (line.find("#" + model + "_INSTANCE_SCALE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					int current = std::atoi(values[0].c_str());
					glm::vec3 scale = glm::vec3((float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()), (float)std::atof(values[3].c_str()));
					m_Models[model].SetScale(scale, current);
				}
				if (line.find("#" + model + "_INSTANCE_EULERANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					int current = std::atoi(values[0].c_str());
					glm::vec3 euler = glm::vec3((float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()), (float)std::atof(values[3].c_str()));
					m_Models[model].SetEulerAngle(euler, current);
				}
			}
			// Load point light
			{
				if (line.find("#POINT_LIGHT_NAME") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					light = values[0];
					AddLight(light, POINT_LIGHT);
				}
				if (line.find("#POINT_LIGHT_" + light + "_POSITION") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetPosition(pos);
				}
				if (line.find("#POINT_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetColor(color);
				}
				if (line.find("#POINT_LIGHT_" + light + "_RANGE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_PointLights[light].SetRange((float)std::atof(values[0].c_str()));
				}
				if (line.find("#POINT_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_PointLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				if (line.find("#POINT_LIGHT_" + light + "_ADS") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 ads = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetADS(ads);
				}
				if (line.find("#POINT_LIGHT_" + light + "_CLQ") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 clq = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetCLQ(clq);
				}
				if (line.find("#POINT_LIGHT_" + light + "_SWITCH") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_PointLights[light].SetLightSwitch((bool)std::atoi(values[0].c_str()));
				}
				if (line.find("#POINT_LIGHT_" + light + "_SHADOW") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_PointLights[light].SetCastShadow((bool)std::atoi(values[0].c_str()));
				}
				if (line.find("#POINT_LIGHT_" + light + "_SHADOW_RES") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_PointLights[light].SetShadowRes((int)std::atoi(values[0].c_str()));
				}
			}
			// Load spot light
			{
				if (line.find("#SPOT_LIGHT_NAME") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					light = values[0];
					AddLight(light, SPOT_LIGHT);
				}
				if (line.find("#SPOT_LIGHT_" + light + "_POSITION") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetPosition(pos);
				}
				if (line.find("#SPOT_LIGHT_" + light + "_EULERANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetEulerAngle(euler);
				}
				if (line.find("#SPOT_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetColor(color);
				}
				if (line.find("#SPOT_LIGHT_" + light + "_RANGE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetRange((float)std::atof(values[0].c_str()));
				}
				if (line.find("#SPOT_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				if (line.find("#SPOT_LIGHT_" + light + "_ANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetAngle((float)std::atof(values[0].c_str()));
				}
				if (line.find("#SPOT_LIGHT_" + light + "_DIMANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetDimAngle((float)std::atof(values[0].c_str()));
				}
				if (line.find("#SPOT_LIGHT_" + light + "_ADS") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 ads = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetADS(ads);
				}
				if (line.find("#SPOT_LIGHT_" + light + "_CLQ") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 clq = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetCLQ(clq);
				}
				if (line.find("#SPOT_LIGHT_" + light + "_SWITCH") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetLightSwitch((bool)std::atoi(values[0].c_str()));
				}
				if (line.find("#SPOT_LIGHT_" + light + "_SHADOW") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetCastShadow((bool)std::atoi(values[0].c_str()));
				}
				if (line.find("#SPOT_LIGHT_" + light + "_SHADOW_RES") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_SpotLights[light].SetShadowRes((int)std::atoi(values[0].c_str()));
				}
			}
			// Load directional light
			{
				if (line.find("#DIR_LIGHT_NAME") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					light = values[0];
					AddLight(light, DIRECTIONAL_LIGHT);
				}
				if (line.find("#DIR_LIGHT_" + light + "_EULERANGLE") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_DirLights[light].SetEulerAngle(euler);
				}
				if (line.find("#DIR_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_DirLights[light].SetColor(color);
				}
				if (line.find("#DIR_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					m_DirLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				if (line.find("#DIR_LIGHT_" + light + "_ADS") != std::string::npos)
				{
					values = rbcore::GetSceneValue(line);
					glm::vec3 ads = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_DirLights[light].SetADS(ads);
				}
			}
		}
		return true;
	}
	return false;
}

void Scene::SaveScene()
{
	std::string savePath;
	if (m_FilePath != "")
	{
		savePath = m_FilePath;
	}
	else
	{
		savePath = SCENE_FILEPATH + m_Name + ".scene";
	}
	std::ofstream stream(savePath);
	std::string line;
	line = "#SCENE_NAME " + m_Name + "\n";
	stream << line;
	// Save Camera
	{
		glm::vec3 pos = m_Camera.GetPosition();
		line = "#CAMERA_POSITION " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
		stream << line;
		glm::vec3 euler = m_Camera.GetEulerAngle();
		line = "#CAMERA_EULERANGLE " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
		stream << line;
	}
	// Save models
	for (unsigned int i = 0; i < m_ModelList.size(); i++)
	{
		std::string model = m_ModelList[i];
		line = "#MODEL_NAME " + model + "\n";
		stream << line;
		line = "#" + model + "_FILEPATH " + m_Models[model].GetFilePath() + "\n";
		stream << line;
		glm::vec3 pos = m_Models[model].GetPosition();
		line = "#" + model + "_POSITION " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
		stream << line;
		glm::vec3 scale = m_Models[model].GetScale();
		line = "#" + model + "_SCALE " + std::to_string(scale.x) + " " + std::to_string(scale.y) + " " + std::to_string(scale.z) + "\n";
		stream << line;
		glm::vec3 euler = m_Models[model].GetEulerAngle();
		line = "#" + model + "_EULERANGLE " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
		stream << line;
		line = "#" + model + "_INSTANCE_NUM " + std::to_string(m_Models[model].GetInstance()) + "\n";
		stream << line;
		line = "#" + model + "_CURRENT " + std::to_string(m_Models[model].GetCurrent()) + "\n";
		stream << line;
		for (unsigned int j = 0; j < m_Models[model].GetInstance(); j++)
		{
			pos = m_Models[model].GetPosition(j + 1);
			line = "#" + model + "_INSTANCE_POSITION " + std::to_string(j + 1) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
			stream << line;
			scale = m_Models[model].GetScale(j + 1);
			line = "#" + model + "_INSTANCE_SCALE " + std::to_string(j + 1) + " " + std::to_string(scale.x) + " " + std::to_string(scale.y) + " " + std::to_string(scale.z) + "\n";
			stream << line;
			euler = m_Models[model].GetEulerAngle(j + 1);
			line = "#" + model + "_INSTANCE_EULERANGLE " + std::to_string(j + 1) + " " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
			stream << line;
		}
	}
	// Save Point Lights
	for (unsigned int i = 0; i < m_PointLightList.size(); i++)
	{
		std::string light = m_PointLightList[i];
		line = "#POINT_LIGHT_NAME " + light + "\n";
		stream << line;
		glm::vec3 pos = m_PointLights[light].GetPosition();
		line = "#POINT_LIGHT_" + light + "_POSITION " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
		stream << line;
		glm::vec3 color = m_PointLights[light].GetColor();
		line = "#POINT_LIGHT_" + light + "_COLOR " + std::to_string(color.x) + " " + std::to_string(color.y) + " " + std::to_string(color.z) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_RANGE " + std::to_string(m_PointLights[light].GetRange()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_INTENSITY " + std::to_string(m_PointLights[light].GetIntensity()) + "\n";
		stream << line;
		glm::vec3 ads = m_PointLights[light].GetADS();
		line = "#POINT_LIGHT_" + light + "_ADS " + std::to_string(ads.x) + " " + std::to_string(ads.y) + " " + std::to_string(ads.z) + "\n";
		stream << line;
		glm::vec3 clq = m_PointLights[light].GetCLQ();
		line = "#POINT_LIGHT_" + light + "_CLQ " + std::to_string(clq.x) + " " + std::to_string(clq.y) + " " + std::to_string(clq.z) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SWITCH " + std::to_string(m_PointLights[light].LightSwitch()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SHADOW " + std::to_string(m_PointLights[light].CastShadow()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SHADOW_RES " + std::to_string(m_PointLights[light].GetShadowRes()) + "\n";
		stream << line;
	}
	// Save Spot Lights
	for (unsigned int i = 0; i < m_SpotLightList.size(); i++)
	{
		std::string light = m_SpotLightList[i];
		line = "#SPOT_LIGHT_NAME " + light + "\n";
		stream << line;
		glm::vec3 pos = m_SpotLights[light].GetPosition();
		line = "#SPOT_LIGHT_" + light + "_POSITION " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
		stream << line;
		glm::vec3 euler = m_SpotLights[light].GetEulerAngle();
		line = "#SPOT_LIGHT_" + light + "_EULERANGLE " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
		stream << line;
		glm::vec3 color = m_SpotLights[light].GetColor();
		line = "#SPOT_LIGHT_" + light + "_COLOR " + std::to_string(color.x) + " " + std::to_string(color.y) + " " + std::to_string(color.z) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_RANGE " + std::to_string(m_SpotLights[light].GetRange()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_INTENSITY " + std::to_string(m_SpotLights[light].GetIntensity()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_ANGLE " + std::to_string(m_SpotLights[light].GetAngle()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_DIMANGLE " + std::to_string(m_SpotLights[light].GetDimAngle()) + "\n";
		stream << line;
		glm::vec3 ads = m_SpotLights[light].GetADS();
		line = "#SPOT_LIGHT_" + light + "_ADS " + std::to_string(ads.x) + " " + std::to_string(ads.y) + " " + std::to_string(ads.z) + "\n";
		stream << line;
		glm::vec3 clq = m_SpotLights[light].GetCLQ();
		line = "#SPOT_LIGHT_" + light + "_CLQ " + std::to_string(clq.x) + " " + std::to_string(clq.y) + " " + std::to_string(clq.z) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SWITCH " + std::to_string(m_SpotLights[light].LightSwitch()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SHADOW " + std::to_string(m_SpotLights[light].CastShadow()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SHADOW_RES " + std::to_string(m_SpotLights[light].GetShadowRes()) + "\n";
		stream << line;
	}
	// Save Directional Lights
	for (unsigned int i = 0; i < m_DirLightList.size(); i++)
	{
		std::string light = m_DirLightList[i];
		line = "#DIR_LIGHT_NAME " + light + "\n";
		stream << line;
		glm::vec3 euler = m_DirLights[light].GetEulerAngle();
		line = "#DIR_LIGHT_" + light + "_EULERANGLE " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
		stream << line;
		glm::vec3 color = m_DirLights[light].GetColor();
		line = "#DIR_LIGHT_" + light + "_COLOR " + std::to_string(color.x) + " " + std::to_string(color.y) + " " + std::to_string(color.z) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_INTENSITY " + std::to_string(m_DirLights[light].GetIntensity()) + "\n";
		stream << line;
		glm::vec3 ads = m_DirLights[light].GetADS();
		line = "#DIR_LIGHT_" + light + "_ADS " + std::to_string(ads.x) + " " + std::to_string(ads.y) + " " + std::to_string(ads.z) + "\n";
		stream << line;
	}
	stream.close();
	rbcore::ShowWarningMsg("Scene saved at: " + savePath);
}

bool Scene::AddModel(std::string filepath)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string name = rbcore::GetFileNameNoSuffix(filepath);
	std::string newName = name;
	while (m_Models.find(newName) != m_Models.end())
	{
		newName = name + "(" + std::to_string(rename) + ")";
		rename++;
	}
	if (rename != 1)
	{
		name = newName;
		spdlog::warn("Model name already exist! Rename to: " + name);
		rbcore::ShowWarningMsg("Model name already exist! Rename to: " + name);
	}

	Model model;
	if (model.LoadModel(name, filepath))
	{
		m_ModelList.push_back(name);
		m_Models.insert(std::pair<std::string, Model>(name, model));
		m_Data.AddModel(name, model);
		rbcore::currentModelScene = nullptr;
		rbcore::currentModelInfo = nullptr;
		return true;
	}
	return false;
}

bool Scene::AddModel(std::string name, std::string filepath)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string newName = name;
	while (m_Models.find(newName) != m_Models.end())
	{
		newName = name + "(" + std::to_string(rename) + ")";
		rename++;
	}
	if (rename != 1)
	{
		name = newName;
		spdlog::warn("Model name already exist! Rename to: " + name);
		rbcore::ShowWarningMsg("Model name already exist! Rename to: " + name);
	}

	Model model;
	if (model.LoadModel(name, filepath))
	{
		m_ModelList.push_back(name);
		m_Models.insert(std::pair<std::string, Model>(name, model));
		m_Data.AddModel(name, model);
		rbcore::currentModelScene = nullptr;
		rbcore::currentModelInfo = nullptr;
		return true;
	}
	return false;
}

bool Scene::DeleteModel(std::string name)
{
	if (std::find(m_ModelList.begin(), m_ModelList.end(), name) != m_ModelList.end())
	{
		m_ModelList.erase(std::remove(m_ModelList.begin(), m_ModelList.end(), name), m_ModelList.end());
		m_ModelList.shrink_to_fit();
		m_Models.erase(name);
		m_Data.DeleteModel(name);
		rbcore::currentModelScene = nullptr;
		rbcore::currentModelInfo = nullptr;
		return true;
	}
	return false;
}

bool Scene::AddLight(std::string name, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		if (AddPointLight(name))
		{
			return true;
		}
		break;
	case SPOT_LIGHT:
		if (AddSpotLight(name))
		{
			return true;
		}
		break;
	case DIRECTIONAL_LIGHT:
		if (AddDirectionalLight(name))
		{
			return true;
		}
		break;
	}
	return false;
}

bool Scene::AddPointLight(std::string name)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string newName = name;
	while (m_PointLights.find(newName) != m_PointLights.end())
	{
		newName = name + "(" + std::to_string(rename) + ")";
		rename++;
	}
	if (rename != 1)
	{
		name = newName;
		spdlog::warn("Light name already exist! Rename to: " + name);
		rbcore::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	PointLight light;
	light.SetName(name);
	m_PointLightList.push_back(name);
	m_PointLights.insert(std::pair<std::string, PointLight>(name, light));
	rbcore::currentPointLight = nullptr;
	return true;
}

bool Scene::AddSpotLight(std::string name)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string newName = name;
	while (m_SpotLights.find(newName) != m_SpotLights.end())
	{
		newName = name + "(" + std::to_string(rename) + ")";
		rename++;
	}
	if (rename != 1)
	{
		name = newName;
		spdlog::warn("Light name already exist! Rename to: " + name);
		rbcore::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	SpotLight light;
	light.SetName(name);
	m_SpotLightList.push_back(name);
	m_SpotLights.insert(std::pair<std::string, SpotLight>(name, light));
	rbcore::currentSpotLight = nullptr;
	return true;
}

bool Scene::AddDirectionalLight(std::string name)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string newName = name;
	while (m_DirLights.find(newName) != m_DirLights.end())
	{
		newName = name + "(" + std::to_string(rename) + ")";
		rename++;
	}
	if (rename != 1)
	{
		name = newName;
		spdlog::warn("Light name already exist! Rename to: " + name);
		rbcore::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	DirectionalLight light;
	light.SetName(name);
	m_DirLightList.push_back(name);
	m_DirLights.insert(std::pair<std::string, DirectionalLight>(name, light));
	rbcore::currentDirLight = nullptr;
	return true;
}

bool Scene::DeleteLight(std::string name, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		if (DeletePointLight(name))
		{
			return true;
		}
		break;
	case SPOT_LIGHT:
		if (DeleteSpotLight(name))
		{
			return true;
		}
		break;
	case DIRECTIONAL_LIGHT:
		if (DeleteDirectionalLight(name))
		{
			return true;
		}
		break;
	}
	return false;
}

bool Scene::DeletePointLight(std::string name)
{
	if (std::find(m_PointLightList.begin(), m_PointLightList.end(), name) != m_PointLightList.end())
	{
		m_PointLightList.erase(std::remove(m_PointLightList.begin(), m_PointLightList.end(), name), m_PointLightList.end());
		m_PointLightList.shrink_to_fit();
		m_PointLights.erase(name);
		rbcore::currentPointLight = nullptr;
		return true;
	}
	return false;
}

bool Scene::DeleteSpotLight(std::string name)
{
	if (std::find(m_SpotLightList.begin(), m_SpotLightList.end(), name) != m_SpotLightList.end())
	{
		m_SpotLightList.erase(std::remove(m_SpotLightList.begin(), m_SpotLightList.end(), name), m_SpotLightList.end());
		m_SpotLightList.shrink_to_fit();
		m_SpotLights.erase(name);
		rbcore::currentSpotLight = nullptr;
		return true;
	}
	return false;
}

bool Scene::DeleteDirectionalLight(std::string name)
{
	if (std::find(m_DirLightList.begin(), m_DirLightList.end(), name) != m_DirLightList.end())
	{
		m_DirLightList.erase(std::remove(m_DirLightList.begin(), m_DirLightList.end(), name), m_DirLightList.end());
		m_DirLightList.shrink_to_fit();
		m_DirLights.erase(name);
		rbcore::currentDirLight = nullptr;
		return true;
	}
	return false;
}

bool Scene::RenameModel(std::string oldName, std::string newName)
{
	if (std::find(m_ModelList.begin(), m_ModelList.end(), oldName) != m_ModelList.end())
	{
		// If name already exist, rename
		unsigned int rename = 1;
		std::string newRename = newName;
		while (m_Models.find(newName) != m_Models.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Model name already exist! Rename to: " + newName);
			rbcore::ShowWarningMsg("Model name already exist! Rename to: " + newName);
		}
		Model model = m_Models[oldName];
		model.SetName(newName);
		m_Models.erase(oldName);
		m_Models.insert(std::pair<std::string, Model>(newName, model));
		for (unsigned int i = 0; i < m_ModelList.size(); i++)
		{
			if (m_ModelList[i] == oldName)
			{
				m_ModelList[i] = newName;
				break;
			}
		}
		m_Data.RenameModel(oldName, newName);
		return true;
	}
	return false;
}

bool Scene::RenameLight(std::string oldName, std::string newName, Light_Type type)
{
	switch (type)
	{
	case POINT_LIGHT:
		if (RenamePointLight(oldName, newName))
		{
			return true;
		}
		break;
	case SPOT_LIGHT:
		if (RenameSpotLight(oldName, newName))
		{
			return true;
		}
		break;
	case DIRECTIONAL_LIGHT:
		if (RenameDirectionalLight(oldName, newName))
		{
			return true;
		}
		break;
	}
	return false;
}

bool Scene::RenamePointLight(std::string oldName, std::string newName)
{
	if (std::find(m_PointLightList.begin(), m_PointLightList.end(), oldName) != m_PointLightList.end())
	{
		// If name already exist, rename
		unsigned int rename = 1;
		std::string newRename = newName;
		while (m_PointLights.find(newName) != m_PointLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			rbcore::ShowWarningMsg("Light name already exist! Rename to: " + newName);
		}
		PointLight light = m_PointLights[oldName];
		light.SetName(newName);
		m_PointLights.erase(oldName);
		m_PointLights.insert(std::pair<std::string, PointLight>(newName, light));
		for (unsigned int i = 0; i < m_PointLightList.size(); i++)
		{
			if (m_PointLightList[i] == oldName)
			{
				m_PointLightList[i] = newName;
				break;
			}
		}
		return true;
	}
	return false;
}

bool Scene::RenameSpotLight(std::string oldName, std::string newName)
{
	if (std::find(m_SpotLightList.begin(), m_SpotLightList.end(), oldName) != m_SpotLightList.end())
	{
		// If name already exist, rename
		unsigned int rename = 1;
		std::string newRename = newName;
		while (m_SpotLights.find(newName) != m_SpotLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			rbcore::ShowWarningMsg("Light name already exist! Rename to: " + newName);
		}
		SpotLight light = m_SpotLights[oldName];
		light.SetName(newName);
		m_SpotLights.erase(oldName);
		m_SpotLights.insert(std::pair<std::string, SpotLight>(newName, light));
		for (unsigned int i = 0; i < m_SpotLightList.size(); i++)
		{
			if (m_SpotLightList[i] == oldName)
			{
				m_SpotLightList[i] = newName;
				break;
			}
		}
		return true;
	}
	return false;
}

bool Scene::RenameDirectionalLight(std::string oldName, std::string newName)
{
	if (std::find(m_DirLightList.begin(), m_DirLightList.end(), oldName) != m_DirLightList.end())
	{
		// If name already exist, rename
		unsigned int rename = 1;
		std::string newRename = newName;
		while (m_DirLights.find(newName) != m_DirLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			rbcore::ShowWarningMsg("Light name already exist! Rename to: " + newName);
		}
		DirectionalLight light = m_DirLights[oldName];
		light.SetName(newName);
		m_DirLights.erase(oldName);
		m_DirLights.insert(std::pair<std::string, DirectionalLight>(newName, light));
		for (unsigned int i = 0; i < m_DirLightList.size(); i++)
		{
			if (m_DirLightList[i] == oldName)
			{
				m_DirLightList[i] = newName;
				break;
			}
		}
		return true;
	}
	return false;
}

void Scene::SetName(std::string name)
{
	m_Name = name;
}

void Scene::DrawUI()
{
	DrawModelsWindow();
	DrawCameraWindow();
	DrawLightsWindow();
}

void Scene::DrawModelsWindow()
{
	if (rbcore::IS_MODELS_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Models", &rbcore::IS_MODELS_OPENED, windowFlags);
		ImGui::LabelHighlighted("Model ");
		if (ImGui::BeginCombo("##Model", rbcore::currentModelScene))
		{
			for (int i = 0; i < m_ModelList.size(); i++)
			{
				bool isSelected = (rbcore::currentModelScene == m_ModelList[i].c_str());
				if (ImGui::Selectable(m_ModelList[i].c_str(), isSelected))
				{
					rbcore::currentModelScene = m_ModelList[i].c_str();
				}
			}
			ImGui::EndCombo();
		}
		if (rbcore::currentModelScene)
		{
			ImGui::CenterAlignWidget("Name", 200.0f);
			ImGui::LabelHighlighted("Name");
			ImGui::PushItemWidth(200.f);
			static char modelName[256] = "";
			strcpy_s(modelName, rbcore::currentModelScene);
			ImGuiInputTextFlags inputFlags = 0;
			inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
			inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
			if (ImGui::InputText("##spotLightName", modelName, IM_ARRAYSIZE(modelName), inputFlags))
			{
				if (modelName != rbcore::currentModelScene)
				{
					RenameModel(rbcore::currentModelScene, modelName);
				}
			}
			m_Models[rbcore::currentModelScene].DrawUI();
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
			ImGui::CenterAlignWidget("Delete");
			if (ImGui::Button("Delete"))
			{
				DeleteModel(rbcore::currentModelScene);
			}
			ImGui::PopStyleColor(1);
		}
		ImGui::End();
	}
}

void Scene::DrawCameraWindow()
{
	if (rbcore::IS_CAMERA_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Camera", &rbcore::IS_CAMERA_OPENED, windowFlags);
		m_Camera.DrawUI();
		ImGui::End();
	}
}

void Scene::DrawLightsWindow()
{
	if (rbcore::IS_LIGHTS_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Lights", &rbcore::IS_LIGHTS_OPENED, windowFlags);
		// Add light
		if (ImGui::TreeNode("Add Light"))
		{
			ImGui::CenterAlignWidget("Name", 200.0f);
			ImGui::LabelHighlighted("Name");
			ImGui::PushItemWidth(200.0f);
			static char lightName[256] = "";
			ImGui::InputText("##LightName", lightName, IM_ARRAYSIZE(lightName));
			ImGui::CenterAlignWidget("Type", 200.0f);
			ImGui::LabelHighlighted("Type");
			ImGui::PushItemWidth(200.0f);
			const char* lightTypeOps[] = {
				"Point Light",
				"Spot Light",
				"Directional Light"
			};
			static int currentLightType = -1;
			ImGui::Combo("##Light_Type", &currentLightType, lightTypeOps, IM_ARRAYSIZE(lightTypeOps));
			ImGui::PopItemWidth();
			ImGui::CenterAlignWidget("Add");
			if (ImGui::Button("Add"))
			{
				if (lightName[0] != '\0' && currentLightType >= 0)
				{
					AddLight(lightName, (Light_Type)currentLightType);
				}
				else
				{
					rbcore::ShowWarningMsg("Incomplete light info!");
				}
				memset(lightName, '\0', sizeof(lightName));
				currentLightType = -1;

			}
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		// Point light
		if (m_PointLightList.size())
		{
			if (ImGui::TreeNode("Point Light"))
			{
				if (ImGui::BeginCombo("##PointLight", rbcore::currentPointLight))
				{
					for (int i = 0; i < m_PointLightList.size(); i++)
					{
						bool isSelected = (rbcore::currentPointLight == m_PointLightList[i].c_str());
						if (ImGui::Selectable(m_PointLightList[i].c_str(), isSelected))
						{
							rbcore::currentPointLight = m_PointLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (rbcore::currentPointLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f);
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f);
					static char pointLightName[256] = "";
					strcpy_s(pointLightName, rbcore::currentPointLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##spotLightName", pointLightName, IM_ARRAYSIZE(pointLightName), inputFlags))
					{
						if (pointLightName != rbcore::currentPointLight)
						{
							RenamePointLight(rbcore::currentPointLight, pointLightName);
						}
					}
					m_PointLights[rbcore::currentPointLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeletePointLight(rbcore::currentPointLight);
					}
					ImGui::PopStyleColor(1);
				}
				ImGui::TreePop();
			}
		}
		// Spot light
		if (m_SpotLightList.size())
		{
			if (ImGui::TreeNode("Spot Light"))
			{
				if (ImGui::BeginCombo("##SpotLight", rbcore::currentSpotLight))
				{
					for (int i = 0; i < m_SpotLightList.size(); i++)
					{
						bool isSelected = (rbcore::currentSpotLight == m_SpotLightList[i].c_str());
						if (ImGui::Selectable(m_SpotLightList[i].c_str(), isSelected))
						{
							rbcore::currentSpotLight = m_SpotLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (rbcore::currentSpotLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f);
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f);
					static char spotLightName[256] = "";
					strcpy_s(spotLightName, rbcore::currentSpotLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##spotLightName", spotLightName, IM_ARRAYSIZE(spotLightName), inputFlags))
					{
						if (spotLightName != rbcore::currentSpotLight)
						{
							RenameSpotLight(rbcore::currentSpotLight, spotLightName);
						}
					}
					m_SpotLights[rbcore::currentSpotLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeleteSpotLight(rbcore::currentSpotLight);
					}
					ImGui::PopStyleColor(1);
				}
				ImGui::TreePop();
			}
		}
		// Directional light
		if (m_DirLightList.size())
		{
			if (ImGui::TreeNode("Directional Light"))
			{
				if (ImGui::BeginCombo("##DirectionalLight", rbcore::currentDirLight))
				{
					for (int i = 0; i < m_DirLightList.size(); i++)
					{
						bool isSelected = (rbcore::currentDirLight == m_DirLightList[i].c_str());
						if (ImGui::Selectable(m_DirLightList[i].c_str(), isSelected))
						{
							rbcore::currentDirLight = m_DirLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (rbcore::currentDirLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f);
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f);
					static char dirLightName[256] = "";
					strcpy_s(dirLightName, rbcore::currentDirLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##spotLightName", dirLightName, IM_ARRAYSIZE(dirLightName), inputFlags))
					{
						if (dirLightName != rbcore::currentDirLight)
						{
							RenameDirectionalLight(rbcore::currentDirLight, dirLightName);
						}
					}
					m_DirLights[rbcore::currentDirLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeleteDirectionalLight(rbcore::currentDirLight);
					}
					ImGui::PopStyleColor(1);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}