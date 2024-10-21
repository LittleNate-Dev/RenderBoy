#include "Data.h"

Data::Data()
{
}

Data::~Data()
{
}

bool Data::Init()
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		if (!m_DataGL.Init())
		{
			return false;
		}
		break;
	default:
		break;
	}
	return true;
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

void Data::ChangeDrawMode()
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.ChangeDrawMode();
		break;
	default:
		break;
	}
}

void Data::AddModel(std::string name, Model& model)
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

void Data::SetShadowRes(std::string name, unsigned int width, unsigned int height, Light_Type type)
{
	switch (core::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.SetShadowRes(name, width, height, type);
		break;
	default:
		break;
	}
}

GLData& Data::GetDataGL()
{
	return m_DataGL;
}