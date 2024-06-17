/*
* -- Project --
*Title:
*	RenderBoy
*Description:
*	RenderBoy is a simple real-time renderer that allows you to view various model formats.
*State:
*	In Development
*Version:
*	0.6.6
*
* -- Dev --
* Name:
*	Nathan Song
* Email:
*	Nathan_Song_Zhijie@outlook.com
* Github:
*	https://github.com/LittleNate-Dev/RenderBoy
*/

#include "app/Application.h"

Application renderBoy;

void BindInput();
void WindowResize(GLFWwindow* window, int width, int height);
void WindowRestore(GLFWwindow* window, int restore);
void WindowIconify(GLFWwindow* window, int iconify);

int main(void)
{
	if (!renderBoy.Init())
	{
		spdlog::error("Failed to launch RenderBoy!");
	}
	else
	{
		BindInput();
		renderBoy.Run();
	}
	return 0;
}

void BindInput()
{
	glfwSetFramebufferSizeCallback(renderBoy.GetWindow(), WindowResize);
	glfwSetWindowIconifyCallback(renderBoy.GetWindow(), WindowIconify);
	glfwSetWindowMaximizeCallback(renderBoy.GetWindow(), WindowRestore);
}

void WindowResize(GLFWwindow* window, int width, int height)
{
	if (width && height)
	{
		core::IS_RENDERBOY_OPENED = true;
		renderBoy.WindowResize(width, height);
		renderBoy.GetScene().GetCamera().SetWindowSize(width, height);
		renderBoy.GetRenderer().ChangeResolution();
	}
}

void WindowRestore(GLFWwindow* window, int restore)
{
	core::IS_RENDERBOY_OPENED = true;
}

void WindowIconify(GLFWwindow* window, int iconify)
{
	core::IS_RENDERBOY_OPENED = false;
}