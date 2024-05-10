#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Application::Application()
{
    m_Window = nullptr;
	m_Width = 0;
	m_Height = 0;
    m_Launched = false;
    m_IconTexID = 0;
}

Application::~Application()
{
    switch (rbcore::SETTINGS.core)
    {
    case OPENGL:
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
        break;
    }
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Application::LoadSettings()
{
    std::ifstream stream(SETTING_FILEPATH);
    std::string line;
    // If setting file doesn't exist, creat a new one
    if (!stream)
    {
        spdlog::warn("Setting file not detected. All RenderBoy Settings will be set to default.");
        rbcore::ShowWarningMsg("Setting file not detected. All RenderBoy Settings will be set to default.");
        return false;
    }
    else
    {
        while (getline(stream, line))
        {
            if (line.find("#WIDTH") != std::string::npos)
            {
                rbcore::SETTINGS.width = std::atoi(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#HEIGHT") != std::string::npos)
            {
                rbcore::SETTINGS.height = std::atoi(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#CORE") != std::string::npos)
            {
                rbcore::SETTINGS.core = (Core)std::atoi(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#FONT_STYLE") != std::string::npos)
            {
                rbcore::SETTINGS.fontStyle = rbcore::FONT_STYLE[(int)std::atoi(rbcore::GetSettingValue(line).c_str())];
            }
            if (line.find("#FONT_SIZE") != std::string::npos)
            {
                rbcore::SETTINGS.fontSize = (int)std::atoi(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#UI_STYLE") != std::string::npos)
            {
                rbcore::SETTINGS.uiStyle = (UIStyle)std::atoi(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#DRAWMODE") != std::string::npos)
            {
                rbcore::SETTINGS.drawMode = (DrawMode)std::atof(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#SHOW_NORMAL") != std::string::npos)
            {
                rbcore::SETTINGS.showNormal = (bool)std::atoi(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#GAMMA") != std::string::npos)
            {
                rbcore::SETTINGS.gamma = (float)std::atof(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#RESOLUTION") != std::string::npos)
            {
                rbcore::SETTINGS.resolution = (float)std::atof(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#ANTI_ALISING") != std::string::npos)
            {
                rbcore::SETTINGS.aa = (AntiAlising)std::atof(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#POST_PROCESS") != std::string::npos)
            {
                rbcore::SETTINGS.pp = (PostProcess)std::atof(rbcore::GetSettingValue(line).c_str());
            }
            if (line.find("#CAMERA_TYPE") != std::string::npos)
            {
                m_Scene.GetCamera().SetCameraType(std::atoi(rbcore::GetSettingValue(line).c_str()));
            }
            if (line.find("#FOV") != std::string::npos)
            {
                m_Scene.GetCamera().SetFOV((float)std::atof(rbcore::GetSettingValue(line).c_str()));
            }
            if (line.find("#NEAR_PLANE") != std::string::npos)
            {
                m_Scene.GetCamera().SetNearPlane((float)std::atof(rbcore::GetSettingValue(line).c_str()));
            }
            if (line.find("#FAR_PLANE") != std::string::npos)
            {
                m_Scene.GetCamera().SetFarPlane((float)std::atof(rbcore::GetSettingValue(line).c_str()));
            }
            if (line.find("#MOVE_SPEED") != std::string::npos)
            {
                m_Scene.GetCamera().SetMoveSpeed((float)std::atof(rbcore::GetSettingValue(line).c_str()));
            }
            if (line.find("#ROTATE_SPEED") != std::string::npos)
            {
                m_Scene.GetCamera().SetRotateSpeed((float)std::atof(rbcore::GetSettingValue(line).c_str()));
            }
        }
        return true;
    }
}

void Application::SaveSettings()
{
    // Addjust window's resolution, in case it's out of bound when it opens next time
    if (rbcore::SETTINGS.width >= 2560 && rbcore::SETTINGS.height >= 1440)
    {
        rbcore::SETTINGS.width = 2560;
        rbcore::SETTINGS.height = 1440;
    }
    else if (rbcore::SETTINGS.width >= 1920 && rbcore::SETTINGS.height >= 1080)
    {
        rbcore::SETTINGS.width = 1920;
        rbcore::SETTINGS.height = 1080;
    }
    else if (rbcore::SETTINGS.width >= 1280 && rbcore::SETTINGS.height >= 720)
    {
        rbcore::SETTINGS.width = 1280;
        rbcore::SETTINGS.height = 720;
    }
    else if (rbcore::SETTINGS.width >= 800 && rbcore::SETTINGS.height >= 600)
    {
        rbcore::SETTINGS.width = 800;
        rbcore::SETTINGS.height = 600;
    }
    else
    {
        rbcore::SETTINGS.width = 540;
        rbcore::SETTINGS.height = 320;
    }
    std::ofstream stream(SETTING_FILEPATH);
    std::string line;
    line = "#WIDTH " + std::to_string(rbcore::SETTINGS.width) + "\n";
    stream << line;
    line = "#HEIGHT " + std::to_string(rbcore::SETTINGS.height) + "\n";
    stream << line;
    line = "#CORE " + std::to_string(rbcore::SETTINGS.core) + "\n";
    stream << line;
    line = "#FONT_STYLE " + std::to_string(rbcore::GetFontStyleIndex(rbcore::SETTINGS.fontStyle)) + "\n";
    stream << line;
    line = "#FONT_SIZE " + std::to_string(rbcore::SETTINGS.fontSize) + "\n";
    stream << line;
    line = "#UI_STYLE " + std::to_string(rbcore::SETTINGS.uiStyle) + "\n";
    stream << line;
    line = "#DRAWMODE " + std::to_string(rbcore::SETTINGS.drawMode) + "\n";
    stream << line;
    line = "#SHOW_NORMAL " + std::to_string(rbcore::SETTINGS.showNormal) + "\n";
    stream << line;
    line = "#GAMMA " + std::to_string(rbcore::SETTINGS.gamma) + "\n";
    stream << line;
    line = "#RESOLUTION " + std::to_string(rbcore::SETTINGS.resolution) + "\n";
    stream << line;
    line = "#ANTI_ALISING " + std::to_string(rbcore::SETTINGS.aa) + "\n";
    stream << line;
    line = "#POST_PROCESS " + std::to_string(rbcore::SETTINGS.pp) + "\n";
    stream << line;
    line = "#CAMERA_TYPE " + std::to_string(m_Scene.GetCamera().GetCameraType()) + "\n";
    stream << line;
    line = "#FOV " + std::to_string(m_Scene.GetCamera().GetFOV()) + "\n";
    stream << line;
    line = "#NEAR_PLANE " + std::to_string(m_Scene.GetCamera().GetNearPlane()) + "\n";
    stream << line;
    line = "#FAR_PLANE " + std::to_string(m_Scene.GetCamera().GetFarPlane()) + "\n";
    stream << line;
    line = "#MOVE_SPEED " + std::to_string(m_Scene.GetCamera().GetMoveSpeed()) + "\n";
    stream << line;
    line = "#ROTATE_SPEED " + std::to_string(m_Scene.GetCamera().GetRotateSpeed()) + "\n";
    stream << line;
    stream.close();
}

bool Application::Init()
{
    // Load RenderBoy Settings
    LoadSettings();

    // Initialize GLEW
    switch (rbcore::SETTINGS.core)
    {
    case Core::OPENGL:
        if (!InitOpenGL())
        {
            return false;
        }
        break;
    }

    // Init renderer and scene
    m_Width = rbcore::SETTINGS.width;
    m_Height = rbcore::SETTINGS.height;
    m_Scene.Init();
    m_Renderer.Init();
    m_Scene.GetCamera().SetWindowSize(m_Width, m_Height);
    return true;
}

bool Application::InitOpenGL()
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        spdlog::error("Failed to initialize GLFW");
        return false;
    }

    // Create a windowed mode window
    m_Window = glfwCreateWindow(rbcore::SETTINGS.width, rbcore::SETTINGS.height, "RenderBoy", nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        spdlog::error("Failed to create window!");
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_Window);

    // Set RenderBoy's Icon
    GLFWimage icons[1];
    unsigned char* iconBuffer;
    iconBuffer = stbi_load("res/icons/Icon_48.png", &icons[0].width, &icons[0].height, 0, 4);
    if (!iconBuffer)
    {
        spdlog::error("RenderBoy icon missing!");
    }
    else
    {
        icons[0].pixels = iconBuffer;
        glfwSetWindowIcon(m_Window, 1, icons);
        stbi_image_free(icons[0].pixels);
    }

    // Initialize GLEW library
    if (glewInit() != GLEW_OK)
    {
        spdlog::error("Failed to initialize GLEW library!");
        return false;
    }
    // Get current OpenGL version
    spdlog::info("Driver Verison: {}", (const char*)glGetString(GL_VERSION));

    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    rbcore::SetUiStyle(rbcore::SETTINGS.uiStyle);
    // Set font
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF(rbcore::GetFontStylePath(rbcore::SETTINGS.fontStyle).c_str(), rbcore::SETTINGS.fontSize);
    io.Fonts->Build();
    // Generate a texture id to display icon
    int iconWidth, iconHeight;
    iconBuffer = stbi_load("res/icons/Icon_256.png", &iconWidth, &iconHeight, 0, 4);
    if (!iconBuffer)
    {
        spdlog::error("RenderBoy icon missing!");
    }
    else
    {
        GLuint iconTexID;
        GLCall(glGenTextures(1, &iconTexID));
        GLCall(glBindTexture(GL_TEXTURE_2D, iconTexID));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iconWidth, iconHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, iconBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        m_IconTexID = (GLuint*)iconTexID;
    }
    if (iconBuffer)
    {
        stbi_image_free(iconBuffer);
    }
    // Initialize file browser
    m_FileBrowser.SetTitle("File Browser");

    return true;
}

void Application::Run()
{
    // render loop
    while (!glfwWindowShouldClose(m_Window)) 
    {
        if (rbcore::IS_RENDERBOY_OPENED)
        {
            Update();
        }
        //Swap front and back buffers
        glfwSwapBuffers(m_Window);
        //Poll for and process events
        glfwPollEvents();
    }
    // When RenderBoy is closed, save settings file
    SaveSettings();
}

void Application::Update()
{
    ReadFilePath();
    // Keyboard and mouse input
    if (!rbcore::IS_SETTINGS_OPENED)
    {
        MouseInput();
        KeyboardInput();
    }
    // Rendering
    m_Renderer.Clear();
    m_Renderer.Draw(m_Scene);
    // Draw UI
    DrawUI();
    LoadFile();
}

bool Application::ReadFilePath()
{
    if (m_FileBrowser.HasSelected())
    {
        std::string filePath = m_FileBrowser.GetSelected().string();
        std::string fileType = rbcore::GetFileFormat(m_FileBrowser.GetSelected().string());
        m_FileBrowser.ClearSelected();
        // The selected file is supported format
        if (fileType == "scene" || rbcore::CheckFileFormat(fileType))
        {
            rbcore::FILEPATH_BUFFER = filePath;
            return true;
        }
        else
        {
            rbcore::ShowWarningMsg("Unsupported file format!");
        }
    }
    return false;
}

bool Application::LoadFile()
{
    // Defer the loading time so loading window can popup
    // Yes, I know this approach is kinda stupid :(
    static int deferLoading = 0;
    if (rbcore::FILEPATH_BUFFER != "" && deferLoading == 20)
    {
        deferLoading = 0;
        switch (rbcore::LOAD_TYPE)
        {
        case RESET_SCENE:
            if (m_Scene.Reset(rbcore::FILEPATH_BUFFER))
            {
                m_Launched = true;
                rbcore::FILEPATH_BUFFER = "";
                rbcore::LOAD_TYPE = NO_FILE;
                return true;
            }
            break;
        case ADD_MODEL:
            if (rbcore::GetFileFormat(rbcore::FILEPATH_BUFFER) == "scene")
            {
                rbcore::FILEPATH_BUFFER = "";
                rbcore::LOAD_TYPE = NO_FILE;
                rbcore::ShowWarningMsg("You can't add a scene to an existing scene!");
                return false;
            }
            else if (m_Scene.AddModel(rbcore::FILEPATH_BUFFER))
            {
                m_Launched = true;
                rbcore::FILEPATH_BUFFER = "";
                rbcore::LOAD_TYPE = NO_FILE;
                return true;
            }
            break;
        }
        rbcore::FILEPATH_BUFFER = "";
        rbcore::LOAD_TYPE = NO_FILE;
        rbcore::ShowWarningMsg("Failed to load file!");
    }
    if (rbcore::FILEPATH_BUFFER != "")
    {
        deferLoading ++;
    }
    
    return false;
}

void Application::DrawUI()
{
    // Change Font size or style
    if (rbcore::RELOAD_FONT)
    {
        rbcore::RELOAD_FONT = false;
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF(rbcore::GetFontStylePath(rbcore::SETTINGS.fontStyle).c_str(), rbcore::SETTINGS.fontSize);
        io.Fonts->Build();
        switch (rbcore::SETTINGS.core)
        {
        case OPENGL:
            ImGui_ImplOpenGL3_DestroyDeviceObjects();
            ImGui_ImplOpenGL3_CreateDeviceObjects();
            break;
        }
    }
    // Draw UI
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    {
        m_FileBrowser.Display();
        if (!m_Launched)
        {
            DrawLaunchWindow();
        }
        else
        {
            DrawMenuBar();
            DrawAboutRenderBoyWindow();
            DrawInfoWindow();
            DrawSettingWindow();
            m_Scene.DrawUI();
        }
        DrawWarningWindow();
        DrawLoadingWindow();
    }
    ImGui::EndFrame();
    ImGui::Render();
    switch (rbcore::SETTINGS.core)
    {
    case OPENGL:
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        break;
    }
}

void Application::DrawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::SetNextItemWidth(4.0f);
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                m_Scene.Reset();
            }
            if (ImGui::MenuItem("Open"))
            {
                rbcore::LOAD_TYPE = RESET_SCENE;
                m_FileBrowser.Open();
            }
            if (ImGui::MenuItem("Add"))
            {
                rbcore::LOAD_TYPE = ADD_MODEL;
                m_FileBrowser.Open();
            }
            if (ImGui::MenuItem("Save"))
            {
                m_Scene.SaveScene();
            }
            if (ImGui::MenuItem("Settings"))
            {
                rbcore::IS_SETTINGS_OPENED = true;
            }
            ImGui::EndMenu();
        }
        ImGui::SetNextItemWidth(4.0f);
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Info"))
            {
                rbcore::IS_INFO_OPENED = true;
            }
            if (ImGui::MenuItem("Camera"))
            {
                rbcore::IS_CAMERA_OPENED = true;
            }
            if (ImGui::MenuItem("Models"))
            {
                rbcore::IS_MODELS_OPENED = true;
            }
            if (ImGui::MenuItem("Lights"))
            {
                rbcore::IS_LIGHTS_OPENED = true;
            }
            ImGui::EndMenu();
        }
        ImGui::SetNextItemWidth(4.0f);
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About RenderBoy"))
            {
                rbcore::IS_ABOUT_OPENED = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
}

void Application::DrawLaunchWindow()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5.0f, io.DisplaySize.y / 3.0f));
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 5.0f * 2.0f, io.DisplaySize.y / 3.0f));
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin("RenderBoy", nullptr, windowFlags);
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::SetCursorPosX((windowSize.x - windowSize.x / 3.0f) * 0.5f);
    if (m_IconTexID)
    {
        ImGui::CenterAlignWidget(windowSize.x / 2.0f);
        ImGui::Image(m_IconTexID, ImVec2(windowSize.x / 2.0f, windowSize.x / 2.0f));
    }
    ImGui::SetCursorPosY(((windowSize.y - windowSize.x / 2.0f) / 4.0f) + windowSize.x / 2.0f);
    ImGui::CenterAlignWidget("RenderBoy");
    ImGui::Text("RenderBoy");
    ImGui::SetCursorPosY(((windowSize.y - windowSize.x / 2.0f) / 4.0f) * 2.0f + windowSize.x / 2.0f);
    ImGui::CenterAlignWidget(APP_VERSION);
    ImGui::Text(APP_VERSION);
    ImGui::SetCursorPosY(((windowSize.y - windowSize.x / 2.0f) / 4.0f) * 3.0f + windowSize.x / 2.0f);
    ImGui::CenterAlignWidget("Open File");
    if (ImGui::Button("Open File"))
    {
        rbcore::LOAD_TYPE = RESET_SCENE;
        m_FileBrowser.Open();
    }
    ImGui::End();
}

void Application::DrawInfoWindow()
{
    if (rbcore::IS_INFO_OPENED)
    {
        //ImGui::SetNextWindowSizeConstraints(ImVec2(270.0f, 90.0f), ImVec2(500.0f, 350.0f));
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoSavedSettings;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("Info", &rbcore::IS_INFO_OPENED, windowFlags);
        //ImGui::SetWindowFontScale(rbcore::FONT_SIZE);
        // FPS
        {
            ImGui::LabelHighlighted("FPS:");
            ImGui::Text("%.1f (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
            static float values[200] = {};
            for (unsigned int i = 0; i < 199; i++)
            {
                values[i] = values[i + 1];
            }
            values[199] = io.Framerate;
            ImGui::PlotLines("##FrameLines", values, IM_ARRAYSIZE(values));
        }
        // Window size
        {
            ImGui::LabelHighlighted("Window Size:");
            ImGui::Text("%d * %d", m_Width, m_Height);
        }
        // Resolution
        {
            ImGui::LabelHighlighted("Resolution:");
            ImGui::Text("%d * %d", (int)(rbcore::SETTINGS.width * rbcore::SETTINGS.resolution), (int)(rbcore::SETTINGS.height * rbcore::SETTINGS.resolution));
        }
        // Driver
        {
            ImGui::LabelHighlighted("Driver:");
            switch (rbcore::SETTINGS.core)
            {
            case OPENGL:
                ImGui::Text((const char*)glGetString(GL_VERSION));
                break;
            }
        }
        // Scene
        if (ImGui::TreeNode("Scene"))
        {
            // Scene
            ImGui::LabelHighlighted("Name");
            ImGui::PushItemWidth(200.f);
            static char sceneName[256] = "";
            strcpy_s(sceneName, m_Scene.GetName().data());
            ImGuiInputTextFlags inputFlags = 0;
            inputFlags |= ImGuiInputTextFlags_CharsNoBlank;
            inputFlags |= ImGuiInputTextFlags_EnterReturnsTrue;
            if (ImGui::InputText("##SceneName", sceneName, IM_ARRAYSIZE(sceneName), inputFlags))
            {
                m_Scene.SetName(sceneName);
            }
            ImGui::PopItemWidth();
            ImGui::LabelHighlighted("Models:");
            ImGui::Text(std::to_string(m_Scene.GetModelList().size()).c_str());
            ImGui::LabelHighlighted("Lights:");
            int lightCount = (int)m_Scene.GetPointLightList().size() +
                             (int)m_Scene.GetSpotLightList().size() +
                             (int)m_Scene.GetDirectionalLightList().size();
            ImGui::Text(std::to_string(lightCount).c_str());
            // Models
            if (m_Scene.GetModelList().size())
            {
                if (ImGui::TreeNode("Models"))
                {
                    if (ImGui::BeginCombo("##ModelInfo", rbcore::currentModelInfo))
                    {
                        for (unsigned int i = 0; i < m_Scene.GetModelList().size(); i++)
                        {
                            bool isSelected = (rbcore::currentModelInfo == m_Scene.GetModelList()[i].c_str());
                            if (ImGui::Selectable(m_Scene.GetModelList()[i].c_str(), isSelected))
                            {
                                rbcore::currentModelInfo = m_Scene.GetModelList()[i].c_str();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    if (rbcore::currentModelInfo)
                    {
                        ImGui::LabelHighlighted("FilePath:");
                        ImGui::TextWrapped(m_Scene.GetModels()[rbcore::currentModelInfo].GetFilePath().c_str());
                        ImGui::LabelHighlighted("Meshes:");
                        ImGui::Text(std::to_string(m_Scene.GetModels()[rbcore::currentModelInfo].GetStatics().meshCount).c_str());
                        ImGui::LabelHighlighted("Triangles:");
                        ImGui::Text(std::to_string(m_Scene.GetModels()[rbcore::currentModelInfo].GetStatics().triangleCount).c_str());
                        ImGui::LabelHighlighted("Vertices:");
                        ImGui::Text(std::to_string(m_Scene.GetModels()[rbcore::currentModelInfo].GetStatics().vertexCount).c_str());
                    }
                    ImGui::TreePop();
                }
            }
            // Lights
            if (m_Scene.GetPointLightList().size() || m_Scene.GetSpotLightList().size() || m_Scene.GetDirectionalLightList().size())
            {
                if (ImGui::TreeNode("Lights"))
                {
                    ImGui::LabelHighlighted("Point Lights:");
                    ImGui::Text(std::to_string(m_Scene.GetPointLightList().size()).c_str());
                    ImGui::LabelHighlighted("Spot Lights:");
                    ImGui::Text(std::to_string(m_Scene.GetSpotLightList().size()).c_str());
                    ImGui::LabelHighlighted("Directional Lights:");
                    ImGui::Text(std::to_string(m_Scene.GetDirectionalLightList().size()).c_str());
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::End();
    }
}

void Application::DrawSettingWindow()
{
    if (rbcore::IS_SETTINGS_OPENED)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Settings", &rbcore::IS_SETTINGS_OPENED, windowFlags);
        ImVec2 displaySize = io.DisplaySize;
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2((displaySize.x - windowSize.x) / 2.0f, (displaySize.y - windowSize.y) / 2.0f));
        // Application
        if (ImGui::TreeNode("Application"))
        {
            // Core
            {
                ImGui::CenterAlignWidget("Core", 120.0f);
                ImGui::LabelHighlighted("Core");
                ImGui::PushItemWidth(120.0f);
                const char* coreOps[] = {
                    "OpenGL",
                    "Vulkan",
                    "DirectX 12"
                };
                static int currentCore = rbcore::SETTINGS.core;
                if (ImGui::Combo("##Core", &currentCore, coreOps, IM_ARRAYSIZE(coreOps)))
                {
                    switch (currentCore)
                    {
                    case Core::OPENGL:
                        rbcore::SETTINGS.core = Core::OPENGL;
                        break;
                    case Core::VULKAN:
                        rbcore::SETTINGS.core = Core::OPENGL;
                        break;
                    case Core::DIRECTX:
                        rbcore::SETTINGS.core = Core::OPENGL;
                        break;
                    default:
                        rbcore::SETTINGS.core = Core::OPENGL;
                        break;
                    }
                }
                ImGui::PopItemWidth();
            }
            // UI style
            {
                ImGui::CenterAlignWidget("UI Style", 180.0f);
                ImGui::LabelHighlighted("UI Style");
                ImGui::PushItemWidth(180.0f);
                const char* uiStyleOps[] = {
                    "Default Light",
                    "Default Dark",
                    "Spectrum"
                };
                static int currentUIStyle = rbcore::SETTINGS.uiStyle;
                if (ImGui::Combo("##UI Style", &currentUIStyle, uiStyleOps, IM_ARRAYSIZE(uiStyleOps)))
                {
                    switch (currentUIStyle)
                    {
                    case UIStyle::DEFAULT_DARK:
                        rbcore::SETTINGS.uiStyle = UIStyle::DEFAULT_DARK;
                        break;
                    case UIStyle::DEFAULT_LIGHT:
                        rbcore::SETTINGS.uiStyle = UIStyle::DEFAULT_LIGHT;
                        break;
                    case UIStyle::SPECTRUM:
                        rbcore::SETTINGS.uiStyle = UIStyle::SPECTRUM;
                        break;
                    default:
                        rbcore::SETTINGS.uiStyle = UIStyle::DEFAULT_LIGHT;
                        break;
                    }
                    rbcore::SetUiStyle(rbcore::SETTINGS.uiStyle);
                }
                ImGui::PopItemWidth();
            }
            // Font Style
            {
                ImGui::CenterAlignWidget("Font Style", 150.0f);
                ImGui::LabelHighlighted("Font Style");
                ImGui::PushItemWidth(150.0f);
                const char* fontStyle = rbcore::SETTINGS.fontStyle.c_str();
                if (ImGui::BeginCombo("##FontStyle", fontStyle))
                {
                    for (int i = 0; i < rbcore::FONT_STYLE.size(); i++)
                    {
                        bool isSelected = (fontStyle == rbcore::FONT_STYLE[i].c_str());
                        if (ImGui::Selectable(rbcore::FONT_STYLE[i].c_str(), isSelected))
                        {
                            fontStyle = rbcore::FONT_STYLE[i].c_str();
                            rbcore::SETTINGS.fontStyle = fontStyle;
                            rbcore::RELOAD_FONT = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            // Font size
            {
                ImGui::CenterAlignWidget("Font Size", 100.0f);
                ImGui::LabelHighlighted("Font Size");
                ImGui::PushItemWidth(100.0f);
                static int fontSize = rbcore::SETTINGS.fontSize;
                if (ImGui::InputInt("##FontSize", &fontSize))
                {
                    if (fontSize <= 0)
                    {
                        fontSize = rbcore::SETTINGS.fontSize;
                    }
                    else
                    {
                        rbcore::SETTINGS.fontSize = fontSize;
                        rbcore::RELOAD_FONT = true;
                    }
                }
                ImGui::PopItemWidth();
            }
            ImGui::TreePop();
        }
        // Visual effects
        if (ImGui::TreeNode("Visual Effects"))
        {
            // Draw mode
            {
                ImGui::CenterAlignWidget("Draw Mode", 150.0f);
                ImGui::LabelHighlighted("Draw Mode");
                ImGui::PushItemWidth(150.0f);
                const char* drawModeOps[] = {
                    "Default",
                    "Wireframe",
                    "Point Cloud",
                    "UV Set",
                    "Depth"
                };
                static int currentDrawMode = rbcore::SETTINGS.drawMode;
                if (ImGui::Combo("##DrawMode", &currentDrawMode, drawModeOps, IM_ARRAYSIZE(drawModeOps)))
                {
                    rbcore::SETTINGS.drawMode = (DrawMode)currentDrawMode;
                }
                ImGui::PopItemWidth();
            }
            // Show normal
            {
                ImGui::CenterAlignWidget("Show Normal");
                ImGui::Checkbox("Show Normal", &rbcore::SETTINGS.showNormal);
                if (rbcore::SETTINGS.showNormal)
                {
                    ImGui::CenterAlignWidget("Normal Color", 200.0f);
                    ImGui::LabelHighlighted("Normal Color");
                    ImGui::PushItemWidth(200.0f);
                    ImGui::ColorEdit3("##NormalColor", &rbcore::NORMAL_COLOR[0]);
                    ImGui::PopItemWidth();
                    ImGui::CenterAlignWidget("Magnitude", 60.0f);
                    ImGui::LabelHighlighted("Magnitude");
                    ImGui::PushItemWidth(60.0f);
                    ImGui::InputFloat("##NormalMagnitude", &rbcore::NORMAL_MAGNITUDE);
                    ImGui::PopItemWidth();
                }
            }
            // Post-Process
            {
                ImGui::CenterAlignWidget("Post-Processing", 120.0f);
                ImGui::LabelHighlighted("Post-Processing");
                ImGui::PushItemWidth(120.0f);
                const char* ppOps[] = {
                    "None",
                    "Inverse",
                    "Blur",
                    "Gray Scale",
                    "Edge"
                };
                static int currentPp = rbcore::SETTINGS.pp;
                if (ImGui::Combo("##PostProcess", &currentPp, ppOps, IM_ARRAYSIZE(ppOps)))
                {
                    rbcore::SETTINGS.pp = (PostProcess)currentPp;
                    m_Renderer.ChangePostProcess();
                }
                ImGui::PopItemWidth();
            }
            ImGui::TreePop();
        }
        // Graphics
        if (ImGui::TreeNode("Graphics"))
        {
            // Projection Type
            {
                ImGui::CenterAlignWidget("Projection Type", 150.0f);
                ImGui::LabelHighlighted("Projection Type");
                ImGui::PushItemWidth(150.0f);
                const char* projOps[] = {
                    "Orthographic",
                    "Persepctive"
                };
                static int currentProj = 1;
                if (ImGui::Combo("##Projection", &currentProj, projOps, IM_ARRAYSIZE(projOps)))
                {
                    m_Scene.GetCamera().SetCameraType(currentProj);
                }
                ImGui::PopItemWidth();
            }
            // Camera FOV
            {
                ImGui::CenterAlignWidget("FOV", 220.0f);
                ImGui::LabelHighlighted("FOV");
                static int fov = (float)m_Scene.GetCamera().GetFOV();
                ImGui::PushItemWidth(220.0f);
                if (ImGui::SliderInt("##FOV", &fov, MIN_FOV, MAX_FOV))
                {
                    m_Scene.GetCamera().SetFOV(fov);
                }
                ImGui::PopItemWidth();
            }
            // Gamma
            {
                ImGui::CenterAlignWidget("Gamma", 150.0f);
                ImGui::LabelHighlighted("Gamma");
                ImGui::PushItemWidth(150.0f);
                ImGui::SliderFloat("##Gamma", &rbcore::SETTINGS.gamma, 0.0f, 3.0f);
                ImGui::PopItemWidth();
            }
            // Resolution
            {
                ImGui::CenterAlignWidget("Resolution", 200.0f);
                ImGui::LabelHighlighted("Resolution");
                ImGui::PushItemWidth(200.0f);
                static int resolution = (int)(100.0f * rbcore::SETTINGS.resolution);
                if (ImGui::SliderInt("##Resolution", &resolution, 20, 400))
                {
                    rbcore::SETTINGS.resolution = (float)resolution / 100.0f;
                    m_Renderer.ChangeResolution();
                }
                ImGui::PopItemWidth();
            }
            // Anti-Alising
            {
                ImGui::CenterAlignWidget("Anti-Alising", 120.0f);
                ImGui::LabelHighlighted("Anti-Alising");
                ImGui::PushItemWidth(120.0f);
                const char* aaOps[] = {
                    "None",
                    "MSAA 4x",
                    "MSAA 8x",
                    "MSAA 16x",
                    "MSAA 32x"
                };
                static int currentAa = rbcore::SETTINGS.aa;
                if (ImGui::Combo("##AntiAlising", &currentAa, aaOps, IM_ARRAYSIZE(aaOps)))
                {
                    rbcore::SETTINGS.aa = (AntiAlising)currentAa;
                    switch (currentAa)
                    {
                    case AntiAlising::MSAA4X:
                        m_Renderer.ChangeMSAA();
                        break;
                    case AntiAlising::MSAA8X:
                        m_Renderer.ChangeMSAA();
                        break;
                    case AntiAlising::MSAA16X:
                        m_Renderer.ChangeMSAA();
                        break;
                    case AntiAlising::MSAA32X:
                        m_Renderer.ChangeMSAA();
                        break;
                    }
                }
                ImGui::PopItemWidth();
            }
            
            ImGui::TreePop();
        }
        ImGui::End();
    }
}

void Application::DrawWarningWindow()
{
    if (rbcore::IS_WARNING_OPENED)
    {
        ImGui::OpenPopup("Warning");
    }
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    windowFlags |= ImGuiWindowFlags_NoScrollbar;
    windowFlags |= ImGuiWindowFlags_NoMove;
    if (ImGui::BeginPopupModal("Warning", &rbcore::IS_WARNING_OPENED, windowFlags)) 
    {
        // Calculate display and window size to center align warning window
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImVec2 displaySize = io.DisplaySize;
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2((displaySize.x - windowSize.x) / 2.0f, (displaySize.y - windowSize.y) / 2.0f));
        ImGui::CenterAlignWidget(rbcore::POPUP_MSG.c_str());
        ImGui::Text(rbcore::POPUP_MSG.c_str());
        ImGui::EndPopup();
    }
}

void Application::DrawLoadingWindow()
{
    if (rbcore::FILEPATH_BUFFER != "")
    {
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        ImGui::Begin("##Loading", nullptr, windowFlags);
        // Calculate display and window size to center align warning window
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImVec2 displaySize = io.DisplaySize;
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2((displaySize.x - windowSize.x) / 2.0f, (displaySize.y - windowSize.y) / 2.0f));
        std::string filename = rbcore::GetFileName(rbcore::FILEPATH_BUFFER);
        ImGui::CenterAlignWidget("Loading " + filename);
        ImGui::Text(("Loading " + filename).c_str());
        ImGui::CenterAlignWidget("Please wait patiently :)");
        ImGui::Text("Please wait patiently :)");
        ImGui::End();
    }
}

void Application::DrawAboutRenderBoyWindow()
{
    if (rbcore::IS_ABOUT_OPENED)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("About RenderBoy", &rbcore::IS_ABOUT_OPENED, windowFlags);
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2((io.DisplaySize.x - windowSize.x) / 2.0f, (io.DisplaySize.y - windowSize.y) / 2.0f));
        ImGui::SetCursorPosX((windowSize.x - windowSize.x / 4.0f) / 2.0f);
        if (m_IconTexID)
        {
            ImGui::Image(m_IconTexID, ImVec2(windowSize.x / 4.0f, windowSize.x / 4.0f));
        }
        ImGui::CenterAlignWidget(APP_VERSION);
        ImGui::Text(APP_VERSION);
        ImGui::CenterAlignWidget(GIT_REPO);
        ImGui::Text(GIT_REPO);
        ImGui::End();
    }
}

void Application::WindowResize(int width, int height)
{
    if (width <= 0)
    {
        width = 1280;
    }
    if (height <= 0)
    {
        height = 720;
    }
    m_Width = width;
    m_Height = height;
    rbcore::SETTINGS.width = width;
    rbcore::SETTINGS.height = height;
}

void Application::KeyboardInput()
{    
    //Camera Movement
    glm::vec3 move = glm::vec4(0.0f);
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) // Forward
    {
        glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(0.0f, 0.0f, -1.0f));
        direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        move =  direction * m_Scene.GetCamera().GetMoveSpeed();
        move += m_Scene.GetCamera().GetPosition();
        m_Scene.GetCamera().SetPosition(move);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) // Backward
    {
        glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(0.0f, 0.0f, 1.0f));
        direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        move = direction * m_Scene.GetCamera().GetMoveSpeed();
        move += m_Scene.GetCamera().GetPosition();
        m_Scene.GetCamera().SetPosition(move);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) // Left
    {
        glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
        direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        move = direction * m_Scene.GetCamera().GetMoveSpeed();
        move += m_Scene.GetCamera().GetPosition();
        m_Scene.GetCamera().SetPosition(move);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) // Right
    {
        glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
        direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
        move = direction * m_Scene.GetCamera().GetMoveSpeed();
        move += m_Scene.GetCamera().GetPosition();
        m_Scene.GetCamera().SetPosition(move);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS) // Up
    {
        move = glm::vec3(0.0f, 1.0f, 0.0f) * m_Scene.GetCamera().GetMoveSpeed();
        move += m_Scene.GetCamera().GetPosition();
        m_Scene.GetCamera().SetPosition(move);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // Down
    {
        move = glm::vec3(0.0f, -1.0f, 0.0f) * m_Scene.GetCamera().GetMoveSpeed();
        move += m_Scene.GetCamera().GetPosition();
        m_Scene.GetCamera().SetPosition(move);
    }
    // Camera Movement

    // Camera Rotation
    if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS) // Roll left
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.z -= 0.5f;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS) // Roll right
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.z += 0.5f;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_UP) == GLFW_PRESS) // Tilt up
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.x += m_Scene.GetCamera().GetRotateSpeed() * 4.0f;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_DOWN) == GLFW_PRESS) // Tilt down
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.x -= m_Scene.GetCamera().GetRotateSpeed() * 4.0f;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS) // Turn left
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.y += m_Scene.GetCamera().GetRotateSpeed() * 4.0f;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Turn right
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.y -= m_Scene.GetCamera().GetRotateSpeed() * 4.0f;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    // Camera Rotation

    // Save Screenshot
    if (glfwGetKey(m_Window, GLFW_KEY_F5) == GLFW_PRESS && m_Launched)
    {
        if (!rbcore::IS_WARNING_OPENED)
        {
            m_Renderer.SaveScreenShot();
        }
    }
    // Save Screenshot
}

void Application::MouseInput()
{
    // Camera Rotation
    if (ImGui::GetIO().MouseDown[1] && (ImGui::GetIO().MousePos.x >= 0.0f && ImGui::GetIO().MousePos.y >= 0.0f))
    {
        glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
        rotate.y += m_Scene.GetCamera().GetRotateSpeed() * ImGui::GetIO().MouseDelta.x;
        rotate.x += m_Scene.GetCamera().GetRotateSpeed() * ImGui::GetIO().MouseDelta.y;
        rotate.x = rotate.x > 89.0f ? 89.0f : rotate.x;
        rotate.x = rotate.x < -89.0f ? -89.0f : rotate.x;
        m_Scene.GetCamera().SetEulerAngle(rotate);
    }
    // Camera Rotation

    // Model Rotation
    if (ImGui::GetIO().MouseDown[2] && (ImGui::GetIO().MousePos.x >= 0.0f && ImGui::GetIO().MousePos.y >= 0.0f))
    {
        if (rbcore::currentModelScene)
        {
            glm::vec3 rotate = m_Scene.GetModels()[rbcore::currentModelScene].GetEulerAngle(m_Scene.GetModels()[rbcore::currentModelScene].GetCurrent());
            rotate.y += 0.5f * ImGui::GetIO().MouseDelta.x;
            rotate.x -= 0.5f * ImGui::GetIO().MouseDelta.y;
            m_Scene.GetModels()[rbcore::currentModelScene].SetEulerAngle(rotate, m_Scene.GetModels()[rbcore::currentModelScene].GetCurrent());
        }
    }
    // Model Rotation
}

GLFWwindow* Application::GetWindow()
{
    return m_Window;
}

Scene& Application::GetScene()
{
    return m_Scene;
}

Renderer& Application::GetRenderer()
{
    return m_Renderer;
}