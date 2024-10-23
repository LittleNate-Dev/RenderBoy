#include "Scene.h"

Scene::Scene()
{
	m_FilePath = "";
	m_Name = "Default";
}

Scene::~Scene()
{
}

bool Scene::Init()
{
	if (!m_Data.Init())
	{
		return false;
	}
	core::SCENE_DATA = &m_Data;
	return true;
}

void Scene::Reset()
{
	m_Name = "Default";
	m_FilePath = "";
	m_Camera.Reset();
	Skybox newSkybox;
	m_Skybox = newSkybox;
	VisualEffects newVFX;
	m_VFX = newVFX;
	std::vector<std::string>().swap(m_ModelList);
	m_Models.clear();
	std::vector<std::string>().swap(m_PointLightList);
	m_PointLights.clear();
	std::vector<std::string>().swap(m_SpotLightList);
	m_SpotLights.clear();
	std::vector<std::string>().swap(m_DirLightList);
	m_DirLights.clear();
	std::vector<std::string>().swap(m_AreaLightList);
	m_AreaLights.clear();
	core::SCENE_STATICS.PointLight = 0;
	core::SCENE_STATICS.SpotLight = 0;
	core::SCENE_STATICS.DirectionalLight = 0;
	core::SCENE_STATICS.AreaLight = 0;
	m_Data.Reset();
	core::currentModelScene = nullptr;
	core::currentPointLight = nullptr;
	core::currentSpotLight = nullptr;
	core::currentDirLight = nullptr;
	core::currentAreaLight = nullptr;
}

bool Scene::Reset(std::string filepath)
{
	std::string fileType = core::GetFileFormat(filepath);
	if (fileType == "scene")
	{
		if (LoadScene(filepath))
		{
			return true;
		}
	}
	else if (core::CheckFileFormatAssimp(fileType))
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
		spdlog::warn("Loading " + core::GetFileName(filepath) + " failed!");
		core::ShowWarningMsg("Loading " + core::GetFileName(filepath) + " failed!");
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
				values = core::GetFileValue(line);
				m_Name = values[0];
			}
			// Load Camera
			{
				if (line.find("#CAMERA_TYPE") != std::string::npos)
				{
					m_Camera.SetCameraType(std::atoi(core::GetFileValue(line)[0].c_str()));
				}
				else if (line.find("#CAMERA_FOV") != std::string::npos)
				{
					m_Camera.SetFOV((float)std::atof(core::GetFileValue(line)[0].c_str()));
				}
				else if (line.find("#CAMERA_NEAR_PLANE") != std::string::npos)
				{
					m_Camera.SetNearPlane((float)std::atof(core::GetFileValue(line)[0].c_str()));
				}
				else if (line.find("#CAMERA_FAR_PLANE") != std::string::npos)
				{
					m_Camera.SetFarPlane((float)std::atof(core::GetFileValue(line)[0].c_str()));
				}
				else if (line.find("#CAMERA_MOVE_SPEED") != std::string::npos)
				{
					m_Camera.SetMoveSpeed((float)std::atof(core::GetFileValue(line)[0].c_str()));
				}
				else if (line.find("#CAMERA_ROTATE_SPEED") != std::string::npos)
				{
					m_Camera.SetRotateSpeed((float)std::atof(core::GetFileValue(line)[0].c_str()));
				}
				else if (line.find("#CAMERA_POSITION") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Camera.SetPosition(pos);
				}
				else if (line.find("#CAMERA_EULERANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Camera.SetEulerAngle(euler);
				}
				else if (line.find("#CAMERA_VFX_EXPOSURE_AUTO") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetAutoExposure((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_EXPOSURE_STRENGTH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetExposure((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_BLOOM_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetBloomSwitch((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_BLOOM_STRENGTH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetBloomStrength((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_BLOOM_FILTER_RADIUS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetBloomFilterRadius((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_DOF_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetFocusSwitch((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_DOF_DISTANCE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetFocusDistance((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_DOF_RANGE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetFocusRange((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#CAMERA_VFX_DOF_FOCAL_LENGTH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Camera.SetFocalLength((float)std::atof(values[0].c_str()));
				}
			}
			// Load Skybox
			{
				if (line.find("#SKYBOX_TYPE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Skybox.Type = (Skybox_Type)std::atoi(values[0].c_str());
				}
				else if (line.find("#SKYBOX_COLOR") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Skybox.Color = color;
				}
				else if (line.find("#SKYBOX_FILEPATH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					std::string filepath;
					for (unsigned int i = 0; i < values.size(); i++)
					{
						filepath += values[i];
						if (i != values.size() - 1)
						{
							filepath += " ";
						}
					}
					std::string directory = core::GetFileDirectory(filepath);
					std::string format = core::GetFileFormat(filepath);
					m_Skybox.Filepath.push_back(directory + "right." + format);
					m_Skybox.Filepath.push_back(directory + "left." + format);
					m_Skybox.Filepath.push_back(directory + "top." + format);
					m_Skybox.Filepath.push_back(directory + "bottom." + format);
					m_Skybox.Filepath.push_back(directory + "front." + format);
					m_Skybox.Filepath.push_back(directory + "back." + format);
					if (!m_Data.LoadSkybox(m_Skybox.Filepath))
					{
						return false;
					}
				}
			}
			// Load VFX
			{
				if (line.find("#SSAO_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_VFX.SSAO = (bool)std::atoi(values[0].c_str());
				}
				else if (line.find("#SSAO_KERNEL_SIZE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_VFX.SSAOKernelSize = (int)std::atoi(values[0].c_str());
				}
				else if (line.find("#SSAO_RADIUS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_VFX.SSAORadius = (float)std::atof(values[0].c_str());
				}
				else if (line.find("#SSAO_BIAS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_VFX.SSAOBias = (float)std::atof(values[0].c_str());
				}
			}
			// Load models
			{
				if (line.find("#MODEL_NAME") != std::string::npos)
				{
					values = core::GetFileValue(line);
					model = values[0];
				}
				else if (line.find("#" + model + "_FILEPATH") != std::string::npos)
				{
					values = core::GetFileValue(line);
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
				else if (line.find("#" + model + "_POSITION") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Models[model].SetPosition(pos);
				}
				else if (line.find("#" + model + "_SCALE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 scale = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Models[model].SetScale(scale);
				}
				else if (line.find("#" + model + "_EULERANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_Models[model].SetEulerAngle(euler);
				}
				else if (line.find("#" + model + "_INSTANCE_NUM") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Models[model].SetInstance(std::atoi(values[0].c_str()));
				}
				else if (line.find("#" + model + "_CURRENT") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_Models[model].SetCurrent(std::atoi(values[0].c_str()));
				}
				else if (line.find("#" + model + "_INSTANCE_POSITION") != std::string::npos)
				{
					values = core::GetFileValue(line);
					int current = std::atoi(values[0].c_str());
					glm::vec3 pos = glm::vec3((float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()), (float)std::atof(values[3].c_str()));
					m_Models[model].SetPosition(pos, current);
				}
				else if (line.find("#" + model + "_INSTANCE_SCALE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					int current = std::atoi(values[0].c_str());
					glm::vec3 scale = glm::vec3((float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()), (float)std::atof(values[3].c_str()));
					m_Models[model].SetScale(scale, current);
				}
				else if (line.find("#" + model + "_INSTANCE_EULERANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					int current = std::atoi(values[0].c_str());
					glm::vec3 euler = glm::vec3((float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()), (float)std::atof(values[3].c_str()));
					m_Models[model].SetEulerAngle(euler, current);
				}
			}
			// Load point light
			{
				if (line.find("#POINT_LIGHT_NAME") != std::string::npos)
				{
					values = core::GetFileValue(line);
					light = values[0];
					AddLight(light, POINT_LIGHT);
				}
				else if (line.find("#POINT_LIGHT_" + light + "_POSITION") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetPosition(pos);
				}
				else if (line.find("#POINT_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetColor(color);
				}
				else if (line.find("#POINT_LIGHT_" + light + "_RANGE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetRange((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_BIAS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetBias((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_ADS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 ads = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetADS(ads);
				}
				else if (line.find("#POINT_LIGHT_" + light + "_CLQ") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 clq = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_PointLights[light].SetCLQ(clq);
				}
				else if (line.find("#POINT_LIGHT_" + light + "_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetLightSwitch((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_SHOW_CUBE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetShowCube((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_CAST_SHADOW") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetCastShadow((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_SHADOW_RES") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetShadowRes((int)std::atoi(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_SOFT_SHADOW") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetSoftShadow((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#POINT_LIGHT_" + light + "_SOFT_DEGREE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_PointLights[light].SetSoftDegree((float)std::atof(values[0].c_str()));
				}
			}
			// Load spot light
			{
				if (line.find("#SPOT_LIGHT_NAME") != std::string::npos)
				{
					values = core::GetFileValue(line);
					light = values[0];
					AddLight(light, SPOT_LIGHT);
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_POSITION") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetPosition(pos);
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_EULERANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetEulerAngle(euler);
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetColor(color);
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_RANGE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetRange((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_ANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetAngle((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_DIMANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetDimAngle((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_BIAS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetBias(glm::vec2((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str())));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_ADS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 ads = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetADS(ads);
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_CLQ") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 clq = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_SpotLights[light].SetCLQ(clq);
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetLightSwitch((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_SHOW_CUBE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetShowCube((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_CAST_SHADOW") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetCastShadow((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_SHADOW_RES") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetShadowRes((int)std::atoi(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_SOFT_SHADOW") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetSoftShadow((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#SPOT_LIGHT_" + light + "_SOFT_DEGREE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_SpotLights[light].SetSoftDegree((float)std::atof(values[0].c_str()));
				}
			}
			// Load directional light
			{
				if (line.find("#DIR_LIGHT_NAME") != std::string::npos)
				{
					values = core::GetFileValue(line);
					light = values[0];
					AddLight(light, DIRECTIONAL_LIGHT);
				}
				else if (line.find("#DIR_LIGHT_" + light + "_EULERANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_DirLights[light].SetEulerAngle(euler);
				}
				else if (line.find("#DIR_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_DirLights[light].SetColor(color);
				}
				else if (line.find("#DIR_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_ADS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 ads = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_DirLights[light].SetADS(ads);
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetLightSwitch((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SHOW_CUBE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetShowCube((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_CAST_SHADOW") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetCastShadow((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SHADOW_RES") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetShadowRes((int)std::atoi(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_CSM_RATIO") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetCSMRatio(glm::vec2((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str())));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SHADOW_BIAS") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 bias = glm::vec3(
						(float)std::atof(values[0].c_str()),
						(float)std::atof(values[1].c_str()),
						(float)std::atof(values[2].c_str()));
					m_DirLights[light].SetBias(bias);
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SHADOW_ENLARGE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetShadowEnlarge((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SOFT_SHADOW") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetSoftShadow((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#DIR_LIGHT_" + light + "_SOFT_DEGREE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_DirLights[light].SetSoftDegree((int)std::atoi(values[0].c_str()));
				}
			}
			// Load area light
			{
				if (line.find("#AREA_LIGHT_NAME") != std::string::npos)
				{
					values = core::GetFileValue(line);
					light = values[0];
					AddLight(light, AREA_LIGHT);
				}
				if (line.find("#AREA_LIGHT_" + light + "_TYPE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					int type = std::atoi(values[0].c_str());
					m_AreaLights[light].SetType((AL_Type)type);
				}
				else if (line.find("#AREA_LIGHT_" + light + "_POSITION") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 pos = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_AreaLights[light].SetPosition(pos);
				}
				else if (line.find("#AREA_LIGHT_" + light + "_EULERANGLE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 euler = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_AreaLights[light].SetEulerAngle(euler);
				}
				else if (line.find("#AREA_LIGHT_" + light + "_SCALE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 scale = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_AreaLights[light].SetScale(scale);
				}
				else if (line.find("#AREA_LIGHT_" + light + "_COLOR") != std::string::npos)
				{
					values = core::GetFileValue(line);
					glm::vec3 color = glm::vec3((float)std::atof(values[0].c_str()), (float)std::atof(values[1].c_str()), (float)std::atof(values[2].c_str()));
					m_AreaLights[light].SetColor(color);
				}
				else if (line.find("#AREA_LIGHT_" + light + "_INTENSITY") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_AreaLights[light].SetIntensity((float)std::atof(values[0].c_str()));
				}
				else if (line.find("#AREA_LIGHT_" + light + "_SWITCH") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_AreaLights[light].SetLightSwitch((bool)std::atoi(values[0].c_str()));
				}
				else if (line.find("#AREA_LIGHT_" + light + "_SHOW_CUBE") != std::string::npos)
				{
					values = core::GetFileValue(line);
					m_AreaLights[light].SetShowCube((bool)std::atoi(values[0].c_str()));
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
		line = "#CAMERA_TYPE " + std::to_string(m_Camera.GetCameraType()) + "\n";
		stream << line;
		line = "#CAMERA_FOV " + std::to_string(m_Camera.GetFOV()) + "\n";
		stream << line;
		line = "#CAMERA_NEAR_PLANE " + std::to_string(m_Camera.GetNearPlane()) + "\n";
		stream << line;
		line = "#CAMERA_FAR_PLANE " + std::to_string(m_Camera.GetFarPlane()) + "\n";
		stream << line;
		line = "#CAMERA_MOVE_SPEED " + std::to_string(m_Camera.GetMoveSpeed()) + "\n";
		stream << line;
		line = "#CAMERA_ROTATE_SPEED " + std::to_string(m_Camera.GetRotateSpeed()) + "\n";
		stream << line;
		glm::vec3 pos = m_Camera.GetPosition();
		line = "#CAMERA_POSITION " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
		stream << line;
		glm::vec3 euler = m_Camera.GetEulerAngle();
		line = "#CAMERA_EULERANGLE " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
		stream << line;
		line = "#CAMERA_VFX_EXPOSURE_AUTO " + std::to_string(m_Camera.GetExposure().Auto) + "\n";
		stream << line;
		line = "#CAMERA_VFX_EXPOSURE_STRENGTH " + std::to_string(m_Camera.GetExposure().Strength) + "\n";
		stream << line;
		line = "#CAMERA_VFX_BLOOM_SWITCH " + std::to_string(m_Camera.GetBloom().Switch) + "\n";
		stream << line;
		line = "#CAMERA_VFX_BLOOM_STRENGTH " + std::to_string(m_Camera.GetBloom().Strength) + "\n";
		stream << line;
		line = "#CAMERA_VFX_BLOOM_FILTER_RADIUS " + std::to_string(m_Camera.GetBloom().FilterRadius) + "\n";
		stream << line;
		line = "#CAMERA_VFX_DOF_SWITCH " + std::to_string(m_Camera.GetFocus().Switch) + "\n";
		stream << line;
		line = "#CAMERA_VFX_DOF_DISTANCE " + std::to_string(m_Camera.GetFocus().Distance) + "\n";
		stream << line;
		line = "#CAMERA_VFX_DOF_RANGE " + std::to_string(m_Camera.GetFocus().Range) + "\n";
		stream << line;
		line = "#CAMERA_VFX_DOF_FOCAL_LENGTH " + std::to_string(m_Camera.GetFocus().FocalLength) + "\n";
		stream << line;
	}
	// Save Skybox
	{
		line = "#SKYBOX_TYPE " + std::to_string(m_Skybox.Type) + "\n";
		stream << line;
		if (m_Skybox.Type == PURE_COLOR)
		{
			line = "#SKYBOX_COLOR " + std::to_string(m_Skybox.Color.x) + " " + std::to_string(m_Skybox.Color.y) + " " + std::to_string(m_Skybox.Color.z) + "\n";
			stream << line;
		}
		else if (m_Skybox.Type == PICTURE && m_Skybox.Filepath.size())
		{
			line = "#SKYBOX_FILEPATH " + m_Skybox.Filepath[0] + "\n";
			stream << line;
		}
	}
	// SaveVFX
	{
		line = "#SSAO_SWITCH " + std::to_string(m_VFX.SSAO) + "\n";
		stream << line;
		line = "#SSAO_KERNEL_SIZE " + std::to_string(m_VFX.SSAOKernelSize) + "\n";
		stream << line;
		line = "#SSAO_RADIUS " + std::to_string(m_VFX.SSAORadius) + "\n";
		stream << line;
		line = "#SSAO_BIAS " + std::to_string(m_VFX.SSAOBias) + "\n";
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
		line = "#POINT_LIGHT_" + light + "_BIAS " + std::to_string(m_PointLights[light].GetBias()) + "\n";
		stream << line;
		glm::vec3 ads = m_PointLights[light].GetADS();
		line = "#POINT_LIGHT_" + light + "_ADS " + std::to_string(ads.x) + " " + std::to_string(ads.y) + " " + std::to_string(ads.z) + "\n";
		stream << line;
		glm::vec3 clq = m_PointLights[light].GetCLQ();
		line = "#POINT_LIGHT_" + light + "_CLQ " + std::to_string(clq.x) + " " + std::to_string(clq.y) + " " + std::to_string(clq.z) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SWITCH " + std::to_string(m_PointLights[light].LightSwitch()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SHOW_CUBE " + std::to_string(m_PointLights[light].ShowCube()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_CAST_SHADOW " + std::to_string(m_PointLights[light].CastShadow()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SHADOW_RES " + std::to_string(m_PointLights[light].GetShadowRes()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SOFT_SHADOW " + std::to_string(m_PointLights[light].SoftShadow()) + "\n";
		stream << line;
		line = "#POINT_LIGHT_" + light + "_SOFT_DEGREE " + std::to_string(m_PointLights[light].GetSoftDegree()) + "\n";
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
		glm::vec2 bias = m_SpotLights[light].GetBias();
		line = "#SPOT_LIGHT_" + light + "_BIAS " + std::to_string(bias.x) + " " + std::to_string(bias.y) + "\n";
		stream << line;
		glm::vec3 ads = m_SpotLights[light].GetADS();
		line = "#SPOT_LIGHT_" + light + "_ADS " + std::to_string(ads.x) + " " + std::to_string(ads.y) + " " + std::to_string(ads.z) + "\n";
		stream << line;
		glm::vec3 clq = m_SpotLights[light].GetCLQ();
		line = "#SPOT_LIGHT_" + light + "_CLQ " + std::to_string(clq.x) + " " + std::to_string(clq.y) + " " + std::to_string(clq.z) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SWITCH " + std::to_string(m_SpotLights[light].LightSwitch()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SHOW_CUBE " + std::to_string(m_SpotLights[light].ShowCube()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_CAST_SHADOW " + std::to_string(m_SpotLights[light].CastShadow()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SHADOW_RES " + std::to_string(m_SpotLights[light].GetShadowRes()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SOFT_SHADOW " + std::to_string(m_SpotLights[light].SoftShadow()) + "\n";
		stream << line;
		line = "#SPOT_LIGHT_" + light + "_SOFT_DEGREE " + std::to_string(m_SpotLights[light].GetSoftDegree()) + "\n";
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
		line = "#DIR_LIGHT_" + light + "_SWITCH " + std::to_string(m_DirLights[light].LightSwitch()) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_SHOW_CUBE " + std::to_string(m_DirLights[light].ShowCube()) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_CAST_SHADOW " + std::to_string(m_DirLights[light].CastShadow()) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_SHADOW_RES " + std::to_string(m_DirLights[light].GetShadowRes()) + "\n";
		stream << line;
		glm::vec2 ratio = m_DirLights[light].GetCSMRatio();
		line = "#DIR_LIGHT_" + light + "_CSM_RATIO " + std::to_string(ratio.x) + " " + std::to_string(ratio.y) + "\n";
		stream << line;
		glm::vec3 bias = m_DirLights[light].GetBias();
		line = "#DIR_LIGHT_" + light + "_SHADOW_BIAS " 
			+ std::to_string(bias.x) + " " 
			+ std::to_string(bias.y) + " "
			+ std::to_string(bias.z) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_SHADOW_ENLARGE " + std::to_string(m_DirLights[light].GetShadowEnlarge()) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_SOFT_SHADOW " + std::to_string(m_DirLights[light].SoftShadow()) + "\n";
		stream << line;
		line = "#DIR_LIGHT_" + light + "_SOFT_DEGREE " + std::to_string(m_DirLights[light].GetSoftDegree()) + "\n";
		stream << line;
	}
	// Save Area Lights
	for (unsigned int i = 0; i < m_AreaLightList.size(); i++)
	{
		std::string light = m_AreaLightList[i];
		line = "#AREA_LIGHT_NAME " + light + "\n";
		stream << line;
		line = "#AREA_LIGHT_" + light + "_TYPE " + std::to_string(m_AreaLights[light].GetLightType()) + "\n";
		stream << line;
		glm::vec3 pos = m_AreaLights[light].GetPosition();
		line = "#AREA_LIGHT_" + light + "_POSITION " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
		stream << line;
		glm::vec3 euler = m_AreaLights[light].GetEulerAngle();
		line = "#AREA_LIGHT_" + light + "_EULERANGLE " + std::to_string(euler.x) + " " + std::to_string(euler.y) + " " + std::to_string(euler.z) + "\n";
		stream << line;
		glm::vec3 scale = m_AreaLights[light].GetScale();
		line = "#AREA_LIGHT_" + light + "_SCALE " + std::to_string(scale.x) + " " + std::to_string(scale.y) + " " + std::to_string(scale.z) + "\n";
		stream << line;
		glm::vec3 color = m_AreaLights[light].GetColor();
		line = "#AREA_LIGHT_" + light + "_COLOR " + std::to_string(color.x) + " " + std::to_string(color.y) + " " + std::to_string(color.z) + "\n";
		stream << line;
		line = "#AREA_LIGHT_" + light + "_INTENSITY " + std::to_string(m_AreaLights[light].GetIntensity()) + "\n";
		stream << line;
		line = "#AREA_LIGHT_" + light + "_TWOSIDE " + std::to_string(m_AreaLights[light].TwoSided()) + "\n";
		stream << line;
		line = "#AREA_LIGHT_" + light + "_SWITCH " + std::to_string(m_AreaLights[light].LightSwitch()) + "\n";
		stream << line;
		line = "#AREA_LIGHT_" + light + "_SHOW_CUBE " + std::to_string(m_AreaLights[light].ShowCube()) + "\n";
		stream << line;
	}
	stream.close();
	core::ShowWarningMsg("Scene saved at: " + savePath);
}

bool Scene::AddModel(std::string filepath)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string name = core::GetFileNameNoSuffix(filepath);
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
		core::ShowWarningMsg("Model name already exist! Rename to: " + name);
	}

	Model model;
	if (model.LoadModel(name, filepath))
	{
		m_ModelList.push_back(name);
		m_Models.insert(std::pair<std::string, Model>(name, model));
		m_Data.AddModel(name, m_Models[name]);
		m_Models[name].SetPosition(m_Camera.GetPosition(), 0);
		core::currentModelScene = nullptr;
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
		core::ShowWarningMsg("Model name already exist! Rename to: " + name);
	}

	Model model;
	if (model.LoadModel(name, filepath))
	{
		m_ModelList.push_back(name);
		m_Models.insert(std::pair<std::string, Model>(name, model));
		m_Data.AddModel(name, m_Models[name]);
		m_Models[name].SetPosition(m_Camera.GetPosition(), 0);
		core::currentModelScene = nullptr;
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
		core::currentModelScene = nullptr;
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
	case AREA_LIGHT:
		if (AddAreaLight(name))
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
		core::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	PointLight light;
	light.SetName(name);
	light.SetPosition(m_Camera.GetPosition());
	m_PointLightList.push_back(name);
	m_PointLights.insert(std::pair<std::string, PointLight>(name, light));
	core::currentPointLight = nullptr;
	core::SCENE_STATICS.PointLight++;
	m_Data.AddLight(name, POINT_LIGHT);
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
		core::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	SpotLight light;
	light.SetName(name);
	light.SetPosition(m_Camera.GetPosition());
	light.SetEulerAngle(glm::vec3(glm::vec2(m_Camera.GetEulerAngle()), 0.0f) + glm::vec3(180.0f, 0.0f, 0.0f));
	m_SpotLightList.push_back(name);
	m_SpotLights.insert(std::pair<std::string, SpotLight>(name, light));
	core::currentSpotLight = nullptr;
	core::SCENE_STATICS.SpotLight++;
	m_Data.AddLight(name, SPOT_LIGHT);
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
		core::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	DirectionalLight light;
	light.SetName(name);
	m_DirLightList.push_back(name);
	m_DirLights.insert(std::pair<std::string, DirectionalLight>(name, light));
	core::currentDirLight = nullptr;
	core::SCENE_STATICS.DirectionalLight++;
	m_Data.AddLight(name, DIRECTIONAL_LIGHT);
	return true;
}

bool Scene::AddAreaLight(std::string name)
{
	// If name already exist, rename
	unsigned int rename = 1;
	std::string newName = name;
	while (m_AreaLights.find(newName) != m_AreaLights.end())
	{
		newName = name + "(" + std::to_string(rename) + ")";
		rename++;
	}
	if (rename != 1)
	{
		name = newName;
		spdlog::warn("Light name already exist! Rename to: " + name);
		core::ShowWarningMsg("Light name already exist! Rename to: " + name);
	}

	AreaLight light;
	light.SetName(name);
	light.SetPosition(m_Camera.GetPosition());
	light.SetEulerAngle(m_Camera.GetEulerAngle());
	//light.SetEulerAngle(glm::vec3(glm::vec2(m_Camera.GetEulerAngle()), 0.0f) + glm::vec3(180.0f, 0.0f, 0.0f));
	m_AreaLightList.push_back(name);
	m_AreaLights.insert(std::pair<std::string, AreaLight>(name, light));
	core::currentAreaLight = nullptr;
	core::SCENE_STATICS.AreaLight++;
    m_Data.AddLight(name, AREA_LIGHT);
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
	case AREA_LIGHT:
		if (DeleteAreaLight(name))
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
		core::currentPointLight = nullptr;
		core::SCENE_STATICS.PointLight--;
		m_Data.DeleteLight(name, POINT_LIGHT);
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
		core::currentSpotLight = nullptr;
		core::SCENE_STATICS.SpotLight--;
		m_Data.DeleteLight(name, SPOT_LIGHT);
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
		core::currentDirLight = nullptr;
		core::SCENE_STATICS.DirectionalLight--;
		m_Data.DeleteLight(name, DIRECTIONAL_LIGHT);
		return true;
	}
	return false;
}

bool Scene::DeleteAreaLight(std::string name)
{
	if (std::find(m_AreaLightList.begin(), m_AreaLightList.end(), name) != m_AreaLightList.end())
	{
		m_AreaLightList.erase(std::remove(m_AreaLightList.begin(), m_AreaLightList.end(), name), m_AreaLightList.end());
		m_AreaLightList.shrink_to_fit();
		m_AreaLights.erase(name);
		core::currentAreaLight = nullptr;
		core::SCENE_STATICS.AreaLight--;
		m_Data.DeleteLight(name, AREA_LIGHT);
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
		while (m_Models.find(newRename) != m_Models.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Model name already exist! Rename to: " + newName);
			core::ShowWarningMsg("Model name already exist! Rename to: " + newName);
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
	case AREA_LIGHT:
		if (RenameAreaLight(oldName, newName))
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
		while (m_PointLights.find(newRename) != m_PointLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			core::ShowWarningMsg("Light name already exist! Rename to: " + newName);
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
		m_Data.RenameLight(oldName, newName, POINT_LIGHT);
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
		while (m_SpotLights.find(newRename) != m_SpotLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			core::ShowWarningMsg("Light name already exist! Rename to: " + newName);
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
		m_Data.RenameLight(oldName, newName, SPOT_LIGHT);
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
		while (m_DirLights.find(newRename) != m_DirLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			core::ShowWarningMsg("Light name already exist! Rename to: " + newName);
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
		m_Data.RenameLight(oldName, newName, DIRECTIONAL_LIGHT);
		return true;
	}
	return false;
}

bool Scene::RenameAreaLight(std::string oldName, std::string newName)
{
	if (std::find(m_AreaLightList.begin(), m_AreaLightList.end(), oldName) != m_AreaLightList.end())
	{
		// If name already exist, rename
		unsigned int rename = 1;
		std::string newRename = newName;
		while (m_AreaLights.find(newRename) != m_AreaLights.end())
		{
			newRename = newName + "(" + std::to_string(rename) + ")";
			rename++;
		}
		if (rename != 1)
		{
			newName = newRename;
			spdlog::warn("Light name already exist! Rename to: " + newName);
			core::ShowWarningMsg("Light name already exist! Rename to: " + newName);
		}
		AreaLight light = m_AreaLights[oldName];
		light.SetName(newName);
		m_AreaLights.erase(oldName);
		m_AreaLights.insert(std::pair<std::string, AreaLight>(newName, light));
		for (unsigned int i = 0; i < m_AreaLightList.size(); i++)
		{
			if (m_AreaLightList[i] == oldName)
			{
				m_AreaLightList[i] = newName;
				break;
			}
		}
		//m_Data.RenameLight(oldName, newName, DIRECTIONAL_LIGHT);
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
	DrawSceneWindow();
	DrawModelsWindow();
	DrawCameraWindow();
	DrawLightsWindow();
}

void Scene::DrawSceneWindow()
{
	if (core::IS_SCENE_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Scene", &core::IS_SCENE_OPENED, windowFlags);
		// rename
		ImGui::LabelHighlighted("Name");
		ImGui::PushItemWidth(200.f * core::GetWidgetWidthCoefficient());
		static char sceneName[256] = "";
		strcpy_s(sceneName, m_Name.data());
		ImGuiInputTextFlags inputFlags = 0;
		inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
		inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
		if (ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName), inputFlags))
		{
			SetName(sceneName);
		}
		ImGui::PopItemWidth();
		// Skybox
		if (ImGui::TreeNode("Skybox"))
		{
			ImGui::CenterAlignWidget("Skybox", 90.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Skybox");
			ImGui::PushItemWidth(90.0f * core::GetWidgetWidthCoefficient());
			const char* skyboxOps[] = {
				"Color",
				"Picture"
			};
			static int currentSkybox = m_Skybox.Type;
			if (currentSkybox != m_Skybox.Type)
			{
				currentSkybox = m_Skybox.Type;
			}
			if (ImGui::Combo("##Skybox", &currentSkybox, skyboxOps, IM_ARRAYSIZE(skyboxOps)))
			{
				m_Skybox.Type = (Skybox_Type)currentSkybox;
			}
			ImGui::PopItemWidth();
			if (m_Skybox.Type)
			{
				ImGui::CenterAlignWidget("Choose Picture");
				if (ImGui::Button("Choose Picture"))
				{
					core::LOAD_TYPE = LOAD_SKYBOX;
					core::FILE_BROWSER.Open();
				}
			}
			else
			{
				ImGui::CenterAlignWidget("Skybox Color", 200.0f * core::GetWidgetWidthCoefficient());
				ImGui::LabelHighlighted("Skybox Color");
				ImGui::PushItemWidth(200.0f * core::GetWidgetWidthCoefficient());
				ImGui::ColorEdit3("##SkyboxColor", &m_Skybox.Color[0]);
				ImGui::PopItemWidth();
			}
			ImGui::TreePop();
		}
		// VFX
		if (ImGui::TreeNode("Visual Effects"))
		{
			// SSAO
			if (ImGui::TreeNode("SSAO"))
			{
				ImGui::CenterAlignWidget("SSAO");
				ImGui::LabelHighlighted("SSAO");
				ImGui::Checkbox("##SSAO", &m_VFX.SSAO);
				if (m_VFX.SSAO)
				{
					// SSAO kernal size
					ImGui::PushItemWidth(120.0f * core::GetWidgetWidthCoefficient());
					ImGui::CenterAlignWidget("Kernel Size", 120.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Kernel Size");
					ImGui::SliderInt("##SSAOKernalSize", &m_VFX.SSAOKernelSize, 1, 64);
					// SSAO Radius
					ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
					ImGui::CenterAlignWidget("Radius", 80.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Radius");
					if (ImGui::InputFloat("##SSAORadius", &m_VFX.SSAORadius, 0.0f, 0.0f, "%.6f"))
					{
						m_VFX.SSAORadius = m_VFX.SSAORadius > 0.0f ? m_VFX.SSAORadius : 0.005f;
					}
					// SSAO Bias
					ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
					ImGui::CenterAlignWidget("Bias", 80.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Bias");
					ImGui::InputFloat("##SSAOBias", &m_VFX.SSAOBias, 0.0f, 0.0f, "%.6f");
				}
				ImGui::TreePop();
			}
			ImGui::CenterAlignWidget("Reset");
			if (ImGui::Button("Reset"))
			{
				VisualEffects newVFX;
				m_VFX = newVFX;
			}
			ImGui::TreePop();
		}
		// Info
		if (ImGui::TreeNode("Info"))
		{
			// Scene
			ImGui::LabelHighlighted("FilePath:");
			ImGui::TextWrapped(m_FilePath.c_str());
			if (m_Skybox.Type == PICTURE)
			{
				ImGui::LabelHighlighted("Skybox FilePath:");
				ImGui::TextWrapped(core::GetFileDirectory(m_Skybox.Filepath[0]).c_str());
			}
			ImGui::LabelHighlighted("Models:");
			ImGui::Text(std::to_string(m_ModelList.size()).c_str());
			ImGui::LabelHighlighted("Point Lights:");
			ImGui::Text(std::to_string(m_PointLightList.size()).c_str());
			ImGui::LabelHighlighted("Spot Lights:");
			ImGui::Text(std::to_string(m_SpotLightList.size()).c_str());
			ImGui::LabelHighlighted("Directional Lights:");
			ImGui::Text(std::to_string(m_DirLightList.size()).c_str());
			ImGui::LabelHighlighted("Area Lights:");
			ImGui::Text(std::to_string(m_AreaLightList.size()).c_str());
			ImGui::TreePop();
		}

		//Debug func
		if (ImGui::Button("Reload Shader"))
		{
			m_Data.GetDataGL().ReInitShader();
		}
		ImGui::End();
	}
}

void Scene::DrawModelsWindow()
{
	if (core::IS_MODELS_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Models", &core::IS_MODELS_OPENED, windowFlags);
		ImGui::LabelHighlighted("Model ");
		if (ImGui::BeginCombo("##Model", core::currentModelScene))
		{
			for (int i = 0; i < m_ModelList.size(); i++)
			{
				bool isSelected = (core::currentModelScene == m_ModelList[i].c_str());
				if (ImGui::Selectable(m_ModelList[i].c_str(), isSelected))
				{
					core::currentModelScene = m_ModelList[i].c_str();
				}
			}
			ImGui::EndCombo();
		}
		if (core::currentModelScene)
		{
			ImGui::CenterAlignWidget("Name", 200.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Name");
			ImGui::PushItemWidth(200.f * core::GetWidgetWidthCoefficient());
			static char modelName[256] = "";
			strcpy_s(modelName, core::currentModelScene);
			ImGuiInputTextFlags inputFlags = 0;
			inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
			inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
			if (ImGui::InputText("##ModelName", modelName, IM_ARRAYSIZE(modelName), inputFlags))
			{
				if (modelName != core::currentModelScene)
				{
					RenameModel(core::currentModelScene, modelName);
				}
			}
			m_Models[core::currentModelScene].DrawUI();
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
			ImGui::CenterAlignWidget("Delete");
			if (ImGui::Button("Delete"))
			{
				DeleteModel(core::currentModelScene);
			}
			ImGui::PopStyleColor(1);
		}
		ImGui::End();
	}
}

void Scene::DrawCameraWindow()
{
	if (core::IS_CAMERA_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Camera", &core::IS_CAMERA_OPENED, windowFlags);
		m_Camera.DrawUI();
		ImGui::End();
	}
}

void Scene::DrawLightsWindow()
{
	if (core::IS_LIGHTS_OPENED)
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("Lights", &core::IS_LIGHTS_OPENED, windowFlags);
		// Add light
		if (ImGui::TreeNode("Add Light"))
		{
			ImGui::PushItemWidth(200.0f * core::GetWidgetWidthCoefficient());
			ImGui::CenterAlignWidget("Name", 200.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Name");
			static char lightName[256] = "";
			ImGui::InputText("##LightName", lightName, IM_ARRAYSIZE(lightName));
			ImGui::CenterAlignWidget("Type", 200.0f * core::GetWidgetWidthCoefficient());
			ImGui::LabelHighlighted("Type");
			const char* lightTypeOps[] = {
				"Point Light",
				"Spot Light",
				"Directional Light",
				"Area Light"
			};
			static int currentLightType = -1;
			ImGui::Combo("##Light_Type", &currentLightType, lightTypeOps, IM_ARRAYSIZE(lightTypeOps));
			ImGui::PopItemWidth();
			if (lightName[0] != '\0' && currentLightType >= 0)
			{
				ImGui::CenterAlignWidget("Add"); 
				if (ImGui::Button("Add"))
				{
					AddLight(lightName, (Light_Type)currentLightType);
					memset(lightName, '\0', sizeof(lightName));
					currentLightType = -1;
				}
			}
			ImGui::TreePop();
		}
		// Point light
		if (m_PointLightList.size())
		{
			if (ImGui::TreeNode("Point Light"))
			{
				if (ImGui::BeginCombo("##PointLight", core::currentPointLight))
				{
					for (int i = 0; i < m_PointLightList.size(); i++)
					{
						bool isSelected = (core::currentPointLight == m_PointLightList[i].c_str());
						if (ImGui::Selectable(m_PointLightList[i].c_str(), isSelected))
						{
							core::currentPointLight = m_PointLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (core::currentPointLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f * core::GetWidgetWidthCoefficient());
					static char pointLightName[256] = "";
					strcpy_s(pointLightName, core::currentPointLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##PointLightName", pointLightName, IM_ARRAYSIZE(pointLightName), inputFlags))
					{
						if (pointLightName != core::currentPointLight)
						{
							RenamePointLight(core::currentPointLight, pointLightName);
						}
					}
					ImGui::PopItemWidth();
					m_PointLights[core::currentPointLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeletePointLight(core::currentPointLight);
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
				if (ImGui::BeginCombo("##SpotLight", core::currentSpotLight))
				{
					for (int i = 0; i < m_SpotLightList.size(); i++)
					{
						bool isSelected = (core::currentSpotLight == m_SpotLightList[i].c_str());
						if (ImGui::Selectable(m_SpotLightList[i].c_str(), isSelected))
						{
							core::currentSpotLight = m_SpotLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (core::currentSpotLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f * core::GetWidgetWidthCoefficient());
					static char spotLightName[256] = "";
					strcpy_s(spotLightName, core::currentSpotLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##SpotLightName", spotLightName, IM_ARRAYSIZE(spotLightName), inputFlags))
					{
						if (spotLightName != core::currentSpotLight)
						{
							RenameSpotLight(core::currentSpotLight, spotLightName);
						}
					}
					ImGui::PopItemWidth();
					m_SpotLights[core::currentSpotLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeleteSpotLight(core::currentSpotLight);
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
				if (ImGui::BeginCombo("##DirectionalLight", core::currentDirLight))
				{
					for (int i = 0; i < m_DirLightList.size(); i++)
					{
						bool isSelected = (core::currentDirLight == m_DirLightList[i].c_str());
						if (ImGui::Selectable(m_DirLightList[i].c_str(), isSelected))
						{
							core::currentDirLight = m_DirLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (core::currentDirLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f * core::GetWidgetWidthCoefficient());
					static char dirLightName[256] = "";
					strcpy_s(dirLightName, core::currentDirLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##DirLightName", dirLightName, IM_ARRAYSIZE(dirLightName), inputFlags))
					{
						if (dirLightName != core::currentDirLight)
						{
							RenameDirectionalLight(core::currentDirLight, dirLightName);
						}
					}
					ImGui::PopItemWidth();
					m_DirLights[core::currentDirLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeleteDirectionalLight(core::currentDirLight);
					}
					ImGui::PopStyleColor(1);
				}
				ImGui::TreePop();
			}
		}
		// Area light
		if (m_AreaLightList.size())
		{
			if (ImGui::TreeNode("Area Light"))
			{
				if (ImGui::BeginCombo("##AreaLight", core::currentAreaLight))
				{
					for (int i = 0; i < m_AreaLightList.size(); i++)
					{
						bool isSelected = (core::currentAreaLight == m_AreaLightList[i].c_str());
						if (ImGui::Selectable(m_AreaLightList[i].c_str(), isSelected))
						{
							core::currentAreaLight = m_AreaLightList[i].c_str();
						}
					}
					ImGui::EndCombo();
				}
				if (core::currentAreaLight)
				{
					ImGui::CenterAlignWidget("Name", 200.0f * core::GetWidgetWidthCoefficient());
					ImGui::LabelHighlighted("Name");
					ImGui::PushItemWidth(200.f * core::GetWidgetWidthCoefficient());
					static char areaLightName[256] = "";
					strcpy_s(areaLightName, core::currentAreaLight);
					ImGuiInputTextFlags inputFlags = 0;
					inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
					inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
					if (ImGui::InputText("##AreaLightName", areaLightName, IM_ARRAYSIZE(areaLightName), inputFlags))
					{
						if (areaLightName != core::currentAreaLight)
						{
							RenameAreaLight(core::currentAreaLight, areaLightName);
						}
					}
					ImGui::PopItemWidth();
					m_AreaLights[core::currentAreaLight].DrawUI();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
					ImGui::CenterAlignWidget("Delete");
					if (ImGui::Button("Delete"))
					{
						DeleteAreaLight(core::currentAreaLight);
					}
					ImGui::PopStyleColor(1);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}