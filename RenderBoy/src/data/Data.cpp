#include "Data.h"

Data::Data()
{
}

Data::~Data()
{
}

void Data::Init()
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.Init();
		break;
	default:
		break;
	}
}

void Data::Reset()
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.Reset();
		break;
	default:
		break;
	}
}

void Data::AddModel(std::string name, Model model)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.AddModel(name, model);
		break;
	default:
		break;
	}
}

void Data::DeleteModel(std::string name)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.DeleteModel(name);
		break;
	default:
		break;
	}
}

void Data::RenameModel(std::string oldName, std::string newName)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.RenameModel(oldName, newName);
		break;
	default:
		break;
	}
}

void Data::AddLight(std::string name, Light_Type type)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.AddLight(name, type);
		break;
	default:
		break;
	}
}

void Data::DeleteLight(std::string name, Light_Type type)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.DeleteLight(name, type);
		break;
	default:
		break;
	}
}

void Data::RenameLight(std::string oldName, std::string newName, Light_Type type)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.RenameLight(oldName, newName, type);
		break;
	default:
		break;
	}
}

bool Data::LoadSkybox(std::vector<std::string> filepath)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		if (m_DataGL.LoadSkybox(filepath))
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

GLData& Data::GetDataGL()
{
	return m_DataGL;
}