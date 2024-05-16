#include "Data.h"

Data::Data()
{
}

Data::~Data()
{
}

void Data::Init()
{
	switch (rbcore::SETTINGS.GraphicsCore)
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
	switch (rbcore::SETTINGS.GraphicsCore)
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
	switch (rbcore::SETTINGS.GraphicsCore)
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
	switch (rbcore::SETTINGS.GraphicsCore)
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
	switch (rbcore::SETTINGS.GraphicsCore)
	{
	case OPENGL:
		m_DataGL.RenameModel(oldName, newName);
		break;
	default:
		break;
	}
}

bool Data::LoadSkybox(std::vector<std::string> filepath)
{
	switch (rbcore::SETTINGS.GraphicsCore)
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