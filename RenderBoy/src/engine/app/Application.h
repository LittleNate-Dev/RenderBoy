/*
* Class:
*	Application
* Description:
*	This class is the RenderToy itself, in the RenderToy, there're a Renderer and a Scene.
*	You first add models to Scene, then pass Scene and your shader to Renderer, then it
*	will draw your models on the screen
*/

#pragma once

#include "core/Core.h"
#include "scene/Scene.h"
#include "renderer/Renderer.h"

class Application
{
private:
	GLFWwindow* m_Window;
	unsigned int m_Width;
	unsigned int m_Height;
	// A file browser used for selecting files. Source: https://github.com/AirGuanZ/imgui-filebrowser
	// App icon texture id
	ImTextureID m_IconTexID;
	// If app is inited and loaded with scene or models, set this value to true;
	bool m_Launched;
	Renderer m_Renderer;
	Scene m_Scene;

	// Load and save RenderToy settings
	bool LoadSettings();
	void SaveSettings();
	// Initialize OpenGL
	bool InitOpenGL();
	// Render Loop
	void Update();
	// Load file
	bool ReadFilePath();
	bool LoadFile();
	// Handle Keyboard and mouse input
	void KeyboardInput();
	void MouseInput();
	// Draw windows
	void DrawLaunchWindow();
	void DrawMenuBar();
	void DrawAboutRenderBoyWindow();
	void DrawPerfWindow();
	void DrawSettingWindow();
	void DrawWarningWindow();
	void DrawLoadingWindow();

public:
	Application();
	~Application();

	// Initialize RenderToy
	bool Init();
	// After initialize application, call this function to run RenderToy
	void Run();
	// Draw UI
	void DrawUI();
	// Resize window resolution
	void WindowResize(int width, int height);
	// Get RenderBoy's members
	inline GLFWwindow* GetWindow()
	{
		return m_Window;
	};
	inline Scene& GetScene()
	{
		return m_Scene;
	};
	inline Renderer& GetRenderer()
	{
		return m_Renderer;
	};
};