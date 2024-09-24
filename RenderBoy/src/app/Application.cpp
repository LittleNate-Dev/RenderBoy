#include "Application.h"

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
    switch (core::SETTINGS.GraphicsCore)
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
        return false;
    }
    else
    {
        while (getline(stream, line))
        {
            if (line.find("#WIDTH") != std::string::npos)
            {
                core::SETTINGS.Width = std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#HEIGHT") != std::string::npos)
            {
                core::SETTINGS.Height = std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#CORE") != std::string::npos)
            {
                core::SETTINGS.GraphicsCore = (Core)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#MOUSE_SENSITIVITY") != std::string::npos)
            {
                core::SETTINGS.Sensitivity = (float)std::atof(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#CONTROLLER_SWITCH") != std::string::npos)
            {
                core::SETTINGS.EnableController = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#CONTROLLER_DEADZONE") != std::string::npos)
            {
                core::SETTINGS.DeadZone.x = (float)std::atof(core::GetFileValue(line)[0].c_str());
                core::SETTINGS.DeadZone.y = (float)std::atof(core::GetFileValue(line)[1].c_str());
            }
            else if (line.find("#FONT_STYLE") != std::string::npos)
            {
                core::SETTINGS.FontStyle = core::FONT_STYLE[(int)std::atoi(core::GetFileValue(line)[0].c_str())];
            }
            else if (line.find("#FONT_SIZE") != std::string::npos)
            {
                core::SETTINGS.FontSize = (int)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#UI_STYLE") != std::string::npos)
            {
                core::SETTINGS.UIStyle = (UI_Style)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#DRAWMODE") != std::string::npos)
            {
                core::SETTINGS.DrawMode = (Draw_Mode)std::atof(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#SHOW_NORMAL") != std::string::npos)
            {
                core::SETTINGS.ShowNormal = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#NORMAL_COLOR") != std::string::npos)
            {
                core::SETTINGS.NormalColor.x = (float)std::atof(core::GetFileValue(line)[0].c_str());
                core::SETTINGS.NormalColor.y = (float)std::atof(core::GetFileValue(line)[1].c_str());
                core::SETTINGS.NormalColor.z = (float)std::atof(core::GetFileValue(line)[2].c_str());
            }
            else if (line.find("#NORMAL_MAGNITUDE") != std::string::npos)
            {
                core::SETTINGS.NormalMagnitude = (float)std::atof(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#GAMMA") != std::string::npos)
            {
                core::SETTINGS.Gamma = (float)std::atof(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#RESOLUTION") != std::string::npos)
            {
                core::SETTINGS.Resolution = (float)std::atof(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#ANTI_ALISING") != std::string::npos)
            {
                core::SETTINGS.AA = (Anti_Alising)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#POST_PROCESS") != std::string::npos)
            {
                core::SETTINGS.PP = (Post_Process)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#PERF_OPENED") != std::string::npos)
            {
                core::IS_PERF_OPENED = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#SCENE_OPENED") != std::string::npos)
            {
                core::IS_SCENE_OPENED = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#MODELS_OPENED") != std::string::npos)
            {
                core::IS_MODELS_OPENED = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#CAMERA_OPENED") != std::string::npos)
            {
                core::IS_CAMERA_OPENED = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
            else if (line.find("#LIGHTS_OPENED") != std::string::npos)
            {
                core::IS_LIGHTS_OPENED = (bool)std::atoi(core::GetFileValue(line)[0].c_str());
            }
        }
        return true;
    }
}

void Application::SaveSettings()
{
    // Addjust window's resolution, in case it's out of bound when it opens next time
    if (core::SETTINGS.Width >= 2560 && core::SETTINGS.Height >= 1440)
    {
        core::SETTINGS.Width = 2560;
        core::SETTINGS.Height = 1440;
    }
    else if (core::SETTINGS.Width >= 1920 && core::SETTINGS.Height >= 1080)
    {
        core::SETTINGS.Width = 1920;
        core::SETTINGS.Height = 1080;
    }
    else if (core::SETTINGS.Width >= 1280 && core::SETTINGS.Height >= 720)
    {
        core::SETTINGS.Width = 1280;
        core::SETTINGS.Height = 720;
    }
    else if (core::SETTINGS.Width >= 800 && core::SETTINGS.Height >= 600)
    {
        core::SETTINGS.Width = 800;
        core::SETTINGS.Height = 600;
    }
    else
    {
        core::SETTINGS.Width = 540;
        core::SETTINGS.Height = 320;
    }
    std::ofstream stream(SETTING_FILEPATH);
    std::string line;
    line = "#WIDTH " + std::to_string(core::SETTINGS.Width) + "\n";
    stream << line;
    line = "#HEIGHT " + std::to_string(core::SETTINGS.Height) + "\n";
    stream << line;
    line = "#CORE " + std::to_string(core::SETTINGS.GraphicsCore) + "\n";
    stream << line;
    line = "#MOUSE_SENSITIVITY " + std::to_string(core::SETTINGS.Sensitivity) + "\n";
    stream << line;
    line = "#CONTROLLER_SWITCH " + std::to_string(core::SETTINGS.EnableController) + "\n";
    stream << line;
    line = "#CONTROLLER_DEADZONE " + std::to_string(core::SETTINGS.DeadZone.x) + " " + std::to_string(core::SETTINGS.DeadZone.y) + "\n";
    stream << line;
    line = "#FONT_STYLE " + std::to_string(core::GetFontStyleIndex(core::SETTINGS.FontStyle)) + "\n";
    stream << line;
    line = "#FONT_SIZE " + std::to_string(core::SETTINGS.FontSize) + "\n";
    stream << line;
    line = "#UI_STYLE " + std::to_string(core::SETTINGS.UIStyle) + "\n";
    stream << line;
    line = "#DRAWMODE " + std::to_string(core::SETTINGS.DrawMode) + "\n";
    stream << line;
    line = "#SHOW_NORMAL " + std::to_string(core::SETTINGS.ShowNormal) + "\n";
    stream << line;
    line = "#NORMAL_COLOR " + std::to_string(core::SETTINGS.NormalColor.x) 
        + " " + std::to_string(core::SETTINGS.NormalColor.y) 
        + " " + std::to_string(core::SETTINGS.NormalColor.z) + "\n";
    stream << line;
    line = "#NORMAL_MAGNITUDE " + std::to_string(core::SETTINGS.NormalMagnitude) + "\n";
    stream << line;
    line = "#GAMMA " + std::to_string(core::SETTINGS.Gamma) + "\n";
    stream << line;
    line = "#RESOLUTION " + std::to_string(core::SETTINGS.Resolution) + "\n";
    stream << line;
    line = "#ANTI_ALISING " + std::to_string(core::SETTINGS.AA) + "\n";
    stream << line;
    line = "#POST_PROCESS " + std::to_string(core::SETTINGS.PP) + "\n";
    stream << line;
    line = "#PERF_OPENED " + std::to_string(core::IS_PERF_OPENED) + "\n";
    stream << line;
    line = "#SCENE_OPENED " + std::to_string(core::IS_SCENE_OPENED) + "\n";
    stream << line;
    line = "#MODELS_OPENED " + std::to_string(core::IS_MODELS_OPENED) + "\n";
    stream << line;
    line = "#CAMERA_OPENED " + std::to_string(core::IS_CAMERA_OPENED) + "\n";
    stream << line;
    line = "#LIGHTS_OPENED " + std::to_string(core::IS_LIGHTS_OPENED) + "\n";
    stream << line;
    stream.close();
}

bool Application::Init()
{
    // Load RenderBoy Settings
    LoadSettings();

    // Initialize GLEW
    switch (core::SETTINGS.GraphicsCore)
    {
    case Core::OPENGL:
        if (!InitOpenGL())
        {
            return false;
        }
        break;
    }

    // Init renderer and scene
    m_Width = core::SETTINGS.Width;
    m_Height = core::SETTINGS.Height;
    core::UpdateRenderRes();
    m_Scene.Init();
    m_Renderer.Init(m_Scene);
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
    m_Window = glfwCreateWindow(core::SETTINGS.Width, core::SETTINGS.Height, "RenderBoy", nullptr, nullptr);
    if (!m_Window)
    {
        glfwTerminate();
        spdlog::error("Failed to create window!");
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_Window);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /*if (core::SETTINGS.FullScreen)
    {
        GLFWmonitor* monitor = glfwGetWindowMonitor(m_Window);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }*/

    // Set RenderBoy's Icon
    FIBITMAP* iconBuffer = FreeImage_Load(FreeImage_GetFileType("res/icons/Icon_48.png", 0), "res/icons/Icon_48.png");
    FreeImage_FlipVertical(iconBuffer);
    FreeImage_ConvertToRGB16(iconBuffer);
    int iconWidth = FreeImage_GetWidth(iconBuffer);
    int iconHeight = FreeImage_GetHeight(iconBuffer);
    GLFWimage icon{ iconWidth, iconHeight, FreeImage_GetBits(iconBuffer) };
    glfwSetWindowIcon(m_Window, 1, &icon);
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
    core::SetUiStyle(core::SETTINGS.UIStyle);
    // Set font
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF(core::GetFontStylePath(core::SETTINGS.FontStyle).c_str(), core::SETTINGS.FontSize);
    io.Fonts->Build();
    // Generate a texture id to display icon
    int logoWidth, logoHeight;
    FIBITMAP* logoBuffer = FreeImage_Load(FreeImage_GetFileType("res/icons/Icon_256.png", 0), "res/icons/Icon_256.png");
    FreeImage_FlipVertical(logoBuffer);
    logoWidth = FreeImage_GetWidth(logoBuffer);
    logoHeight = FreeImage_GetHeight(logoBuffer);
    if (!logoBuffer)
    {
        spdlog::error("RenderBoy icon missing!");
    }
    else
    {
        GLuint logoTexID;
        GLCall(glGenTextures(1, &logoTexID));
        GLCall(glBindTexture(GL_TEXTURE_2D, logoTexID));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, logoWidth, logoHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(logoBuffer)));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        m_IconTexID = (GLuint*)logoTexID;
    }
    if (logoBuffer)
    {
        FreeImage_Unload(logoBuffer);
    }
    // Initialize file browser
    core::FILE_BROWSER.SetTitle("File Browser");

    return true;
}

void Application::Run()
{
    // render loop
    while (!glfwWindowShouldClose(m_Window)) 
    {
        core::CURRENT_TIME = glfwGetTime();
        core::TIME_DELTA = core::CURRENT_TIME - core::LAST_TIME;
        core::LAST_TIME = core::CURRENT_TIME;
        if (core::IS_RENDERBOY_OPENED)
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
    // Rendering
    m_Renderer.Clear();
    if (m_Launched)
    {
        m_Renderer.Draw(m_Scene);
    }
    // Keyboard and mouse input
    MouseInput();
    KeyboardInput();
    if (core::SETTINGS.EnableController)
    {
        GamepadInput();
    }
    // Draw UI
    DrawUI();
    LoadFile();
}

bool Application::ReadFilePath()
{
    if (core::FILE_BROWSER.HasSelected())
    {
        std::string filePath = core::FILE_BROWSER.GetSelected().string();
        std::string fileType = core::GetFileFormat(core::FILE_BROWSER.GetSelected().string());
        core::FILE_BROWSER.ClearSelected();
        // The selected file is supported format
        if (fileType == "scene" || core::CheckFileFormat(fileType))
        {
            core::FILEPATH_BUFFER = filePath;
            return true;
        }
        else
        {
            core::ShowWarningMsg("Unsupported file format!");
        }
    }
    return false;
}

bool Application::LoadFile()
{
    // Defer the loading time so loading window can popup
    // Yes, I know this approach is kinda stupid :(
    static int deferLoading = 0;
    if (core::FILEPATH_BUFFER != "" && deferLoading == 20)
    {
        deferLoading = 0;
        switch (core::LOAD_TYPE)
        {
        case RESET_SCENE:
            if (m_Scene.Reset(core::FILEPATH_BUFFER))
            {
                m_Launched = true;
                core::FILEPATH_BUFFER = "";
                core::LOAD_TYPE = NO_FILE;
                return true;
            }
            break;
        case ADD_MODEL:
            if (core::GetFileFormat(core::FILEPATH_BUFFER) == "scene")
            {
                core::FILEPATH_BUFFER = "";
                core::LOAD_TYPE = NO_FILE;
                core::ShowWarningMsg("You can't add a scene to an existing scene!");
                return false;
            }
            else if (core::CheckFileFormatAssimp(core::GetFileFormat(core::FILEPATH_BUFFER)) && m_Scene.AddModel(core::FILEPATH_BUFFER))
            {
                m_Launched = true;
                core::FILEPATH_BUFFER = "";
                core::LOAD_TYPE = NO_FILE;
                return true;
            }
            break;
        case LOAD_SKYBOX:
            std::vector<std::string> skyboxName({
                "left", "right",
                "top", "bottom",
                "front", "back"
                });
            if (core::GetFileFormat(core::FILEPATH_BUFFER) == "scene" || core::CheckFileFormatAssimp(core::GetFileFormat(core::FILEPATH_BUFFER)))
            {
                core::FILEPATH_BUFFER = "";
                core::LOAD_TYPE = NO_FILE;
                core::ShowWarningMsg("You have to choose a picture for skybox!");
                return false;
            }
            else if (std::find(skyboxName.begin(), skyboxName.end(), core::GetFileNameNoSuffix(core::FILEPATH_BUFFER)) != skyboxName.end())
            {
                std::string directory = core::GetFileDirectory(core::FILEPATH_BUFFER);
                std::string format = core::GetFileFormat(core::FILEPATH_BUFFER);
                std::vector<std::string> filepath;
                filepath.push_back(directory + "right." + format);
                filepath.push_back(directory + "left." + format);
                filepath.push_back(directory + "top." + format);
                filepath.push_back(directory + "bottom." + format);
                filepath.push_back(directory + "front." + format);
                filepath.push_back(directory + "back." + format);
                if (m_Scene.GetData().LoadSkybox(filepath))
                {
                    core::FILEPATH_BUFFER = "";
                    core::LOAD_TYPE = NO_FILE;
                    m_Scene.GetSkybox().Filepath = filepath;
                    return true;
                }
            }
            else
            {
                core::FILEPATH_BUFFER = "";
                core::LOAD_TYPE = NO_FILE;
                core::ShowWarningMsg("Incorrect file name! Check README for more details.");
                core::IS_ABOUT_OPENED = true;
                return false;
            }
            break;
        }
        core::FILEPATH_BUFFER = "";
        core::LOAD_TYPE = NO_FILE;
        core::ShowWarningMsg("Failed to load file!");
    }
    if (core::FILEPATH_BUFFER != "")
    {
        deferLoading ++;
    }
    
    return false;
}

void Application::DrawUI()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Change Font size or style
    if (core::RELOAD_FONT)
    {
        core::RELOAD_FONT = false;
        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF(core::GetFontStylePath(core::SETTINGS.FontStyle).c_str(), core::SETTINGS.FontSize);
        io.Fonts->Build();
        switch (core::SETTINGS.GraphicsCore)
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
        if (core::IS_UI_OPENED)
        {
            core::FILE_BROWSER.Display();
            DrawSettingWindow();
        }
        if (!m_Launched && !core::IS_SETTINGS_OPENED)
        {
            DrawLaunchWindow();
        }
        else if (m_Launched && core::IS_UI_OPENED)
        {
            DrawMenuBar();
            DrawAboutRenderBoyWindow();
            DrawPerfWindow();
            //DrawSettingWindow();
            m_Scene.DrawUI();
        }
        if (core::IS_UI_OPENED)
        {
            DrawWarningWindow();
            DrawLoadingWindow();
        }
    }
    ImGui::EndFrame();
    ImGui::Render();
    switch (core::SETTINGS.GraphicsCore)
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
        ImGui::SetNextItemWidth(4.0f * core::GetWidgetWidthCoefficient());
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New"))
            {
                m_Scene.Reset();
            }
            if (ImGui::MenuItem("Open"))
            {
                core::LOAD_TYPE = RESET_SCENE;
                core::FILE_BROWSER.Open();
            }
            if (ImGui::MenuItem("Add"))
            {
                core::LOAD_TYPE = ADD_MODEL;
                core::FILE_BROWSER.Open();
            }
            if (ImGui::MenuItem("Save"))
            {
                m_Scene.SaveScene();
            }
            if (ImGui::MenuItem("Settings"))
            {
                core::IS_SETTINGS_OPENED = true;
            }
            ImGui::EndMenu();
        }
        ImGui::SetNextItemWidth(4.0f * core::GetWidgetWidthCoefficient());
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Perf"))
            {
                core::IS_PERF_OPENED = true;
            }
            if (ImGui::MenuItem("Scene"))
            {
                core::IS_SCENE_OPENED = true;
            }
            if (ImGui::MenuItem("Camera"))
            {
                core::IS_CAMERA_OPENED = true;
            }
            if (ImGui::MenuItem("Models"))
            {
                core::IS_MODELS_OPENED = true;
            }
            if (ImGui::MenuItem("Lights"))
            {
                core::IS_LIGHTS_OPENED = true;
            }
            ImGui::EndMenu();
        }
        ImGui::SetNextItemWidth(4.0f * core::GetWidgetWidthCoefficient());
        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About RenderBoy"))
            {
                core::IS_ABOUT_OPENED = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
}

void Application::DrawLaunchWindow()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoScrollbar;
    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("RenderBoy", nullptr, windowFlags);
    ImVec2 displaySize = io.DisplaySize;
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::SetWindowPos(ImVec2((displaySize.x - windowSize.x) / 2.0f, (displaySize.y - windowSize.y) / 2.0f));
    if (m_IconTexID)
    {
        ImGui::CenterAlignWidget(displaySize.x / 8.0f);
        ImGui::Image(m_IconTexID, ImVec2(displaySize.x / 8.0f, displaySize.x / 8.0f));
    }
    ImGui::CenterAlignWidget("RenderBoy");
    ImGui::Text("RenderBoy");
    ImGui::CenterAlignWidget(APP_VERSION);
    ImGui::Text(APP_VERSION);
    ImGui::CenterAlignWidget("Open");
    if (ImGui::Button("Open"))
    {
        core::LOAD_TYPE = RESET_SCENE;
        core::FILE_BROWSER.Open();
    }
    ImGui::CenterAlignWidget("Settings");
    if (ImGui::Button("Settings"))
    {
        core::IS_SETTINGS_OPENED = true;
    }
    ImGui::End();
}

void Application::DrawPerfWindow()
{
    if (core::IS_PERF_OPENED)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("Performance", &core::IS_PERF_OPENED, windowFlags);
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
            ImGui::Text("%d * %d", (int)(core::SETTINGS.Width * core::SETTINGS.Resolution), (int)(core::SETTINGS.Height * core::SETTINGS.Resolution));
        }
        // Driver
        {
            ImGui::LabelHighlighted("Driver:");
            switch (core::SETTINGS.GraphicsCore)
            {
            case OPENGL:
                ImGui::Text((const char*)glGetString(GL_VERSION));
                break;
            }
        }
        ImGui::End();
    }
}

void Application::DrawSettingWindow()
{
    if (core::IS_SETTINGS_OPENED)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Settings", &core::IS_SETTINGS_OPENED, windowFlags);
        ImVec2 displaySize = io.DisplaySize;
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2((displaySize.x - windowSize.x) / 2.0f, (displaySize.y - windowSize.y) / 2.0f));
        // Application
        if (ImGui::TreeNode("Application"))
        {
            // Core
            /*{
                ImGui::CenterAlignWidget("Core", 100.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Core");
                ImGui::PushItemWidth(100.0f * core::GetWidgetWidthCoefficient());
                const char* coreOps[] = {
                    "OpenGL",
                    "Vulkan",
                    "DirectX 12"
                };
                static int currentCore = core::SETTINGS.GraphicsCore;
                if (ImGui::Combo("##Core", &currentCore, coreOps, IM_ARRAYSIZE(coreOps)))
                {
                    switch (currentCore)
                    {
                    case Core::OPENGL:
                        core::SETTINGS.GraphicsCore = Core::OPENGL;
                        break;
                    case Core::VULKAN:
                        core::SETTINGS.GraphicsCore = Core::OPENGL;
                        break;
                    case Core::DIRECTX:
                        core::SETTINGS.GraphicsCore = Core::OPENGL;
                        break;
                    default:
                        core::SETTINGS.GraphicsCore = Core::OPENGL;
                        break;
                    }
                }
                ImGui::PopItemWidth();
            }*/
            // UI style
            {
                ImGui::CenterAlignWidget("UI Style", 120.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("UI Style");
                ImGui::PushItemWidth(120.0f * core::GetWidgetWidthCoefficient());
                const char* uiStyleOps[] = {
                    "Default Light",
                    "Default Dark",
                    "Spectrum"
                };
                static int currentUIStyle;
                currentUIStyle = core::SETTINGS.UIStyle;
                if (ImGui::Combo("##UI Style", &currentUIStyle, uiStyleOps, IM_ARRAYSIZE(uiStyleOps)))
                {
                    switch (currentUIStyle)
                    {
                    case UI_Style::DEFAULT_DARK:
                        core::SETTINGS.UIStyle = UI_Style::DEFAULT_DARK;
                        break;
                    case UI_Style::DEFAULT_LIGHT:
                        core::SETTINGS.UIStyle = UI_Style::DEFAULT_LIGHT;
                        break;
                    case UI_Style::SPECTRUM:
                        core::SETTINGS.UIStyle = UI_Style::SPECTRUM;
                        break;
                    default:
                        core::SETTINGS.UIStyle = UI_Style::DEFAULT_LIGHT;
                        break;
                    }
                    core::SetUiStyle(core::SETTINGS.UIStyle);
                }
                ImGui::PopItemWidth();
            }
            // Font Style
            {
                ImGui::CenterAlignWidget("Font Style", 100.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Font Style");
                ImGui::PushItemWidth(100.0f * core::GetWidgetWidthCoefficient());
                const char* fontStyle = core::SETTINGS.FontStyle.c_str();
                if (ImGui::BeginCombo("##FontStyle", fontStyle))
                {
                    for (int i = 0; i < core::FONT_STYLE.size(); i++)
                    {
                        bool isSelected = (fontStyle == core::FONT_STYLE[i].c_str());
                        if (ImGui::Selectable(core::FONT_STYLE[i].c_str(), isSelected))
                        {
                            fontStyle = core::FONT_STYLE[i].c_str();
                            core::SETTINGS.FontStyle = fontStyle;
                            core::RELOAD_FONT = true;
                        }
                    }
                    ImGui::EndCombo();
                }
            }
            // Font size
            {
                ImGui::CenterAlignWidget("Font Size", 90.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Font Size");
                ImGui::PushItemWidth(90.0f * core::GetWidgetWidthCoefficient());
                static int fontSize = core::SETTINGS.FontSize;
                if (ImGui::InputInt("##FontSize", &fontSize))
                {
                    if (fontSize <= 0)
                    {
                        fontSize = core::SETTINGS.FontSize;
                    }
                    else
                    {
                        core::SETTINGS.FontSize = fontSize;
                        core::RELOAD_FONT = true;
                    }
                }
                ImGui::PopItemWidth();
            }
            ImGui::TreePop();
        }
        // Control
        if (ImGui::TreeNode("Control"))
        {
            // Mouse
            if (ImGui::TreeNode("Mouse"))
            {
                // Sensitivity
                {
                    ImGui::CenterAlignWidget("Sensitivity", 220.0f * core::GetWidgetWidthCoefficient());
                    ImGui::LabelHighlighted("Sensitivity");
                    ImGui::PushItemWidth(220.0f * core::GetWidgetWidthCoefficient());
                    ImGui::SliderFloat("##MouseSensitivity", &core::SETTINGS.Sensitivity, SETTING_MOUSE_MIN_SEN, SETTING_MOUSE_MAX_SEN);
                }
                ImGui::TreePop();
            }
            // Controller
            if (ImGui::TreeNode("Controller"))
            {
                // Enable controller
                {
                    ImGui::CenterAlignWidget("Enable Controller");
                    ImGui::LabelHighlighted("Enable Controller");
                    ImGui::Checkbox("##EnableController", &core::SETTINGS.EnableController);
                }
                // Dead zone
                if (core::SETTINGS.EnableController)
                {
                    ImGui::CenterAlignWidget("Deadzone", 80.0f * core::GetWidgetWidthCoefficient());
                    ImGui::LabelHighlighted("Deadzone");
                    ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
                    if (ImGui::InputFloat("##DeadzoneLeft", &core::SETTINGS.DeadZone.x, 0.0f, 0.0f, "%.6f"))
                    {
                        core::SETTINGS.DeadZone.x = core::SETTINGS.DeadZone.x < 0.0f ? 0.0f : core::SETTINGS.DeadZone.x;
                    } ImGui::SameLine();
                    if (ImGui::InputFloat("##DeadzoneRight", &core::SETTINGS.DeadZone.y, 0.0f, 0.0f, "%.6f"))
                    {
                        core::SETTINGS.DeadZone.y = core::SETTINGS.DeadZone.y < 0.0f ? 0.0f : core::SETTINGS.DeadZone.y;
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        // Visual effects
        if (ImGui::TreeNode("Visual Effects"))
        {
            // Draw mode
            {
                ImGui::CenterAlignWidget("Draw Mode", 120.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Draw Mode");
                ImGui::PushItemWidth(120.0f * core::GetWidgetWidthCoefficient());
                const char* drawModeOps[] = {
                    "Default",
                    "Blank",
                    "Wireframe",
                    "Point Cloud",
                    "UV Set",
                    "Depth",
                    "Normal"
                };
                static int currentDrawMode;
                currentDrawMode = core::SETTINGS.DrawMode;
                if (ImGui::Combo("##Draw_Mode", &currentDrawMode, drawModeOps, IM_ARRAYSIZE(drawModeOps)))
                {
                    core::SETTINGS.DrawMode = (Draw_Mode)currentDrawMode;
                    m_Scene.GetData().ChangeDrawMode();
                }
                ImGui::PopItemWidth();
            }
            // Post-Process
            {
                ImGui::CenterAlignWidget("Post-Processing", 105.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Post-Processing");
                ImGui::PushItemWidth(105.0f * core::GetWidgetWidthCoefficient());
                const char* ppOps[] = {
                    "None",
                    "Inverse",
                    "Blur",
                    "Gray Scale",
                    "Edge"
                };
                static int currentPp;
                currentPp = core::SETTINGS.PP;
                if (ImGui::Combo("##Post_Process", &currentPp, ppOps, IM_ARRAYSIZE(ppOps)))
                {
                    core::SETTINGS.PP = (Post_Process)currentPp;
                    m_Renderer.ChangePostProcess();
                }
                ImGui::PopItemWidth();
            }
            // Show normal
            if (ImGui::TreeNode("Normal"))
            {
                ImGui::CenterAlignWidget("Show Normal");
                ImGui::LabelHighlighted("Show Normal");
                ImGui::Checkbox("##ShowNormal", &core::SETTINGS.ShowNormal);
                if (core::SETTINGS.ShowNormal)
                {
                    ImGui::CenterAlignWidget("Normal Color", 200.0f * core::GetWidgetWidthCoefficient());
                    ImGui::LabelHighlighted("Normal Color");
                    ImGui::PushItemWidth(200.0f * core::GetWidgetWidthCoefficient());
                    ImGui::ColorEdit3("##NormalColor", &core::SETTINGS.NormalColor[0]);
                    ImGui::PopItemWidth();
                    ImGui::CenterAlignWidget("Magnitude", 60.0f * core::GetWidgetWidthCoefficient());
                    ImGui::LabelHighlighted("Magnitude");
                    ImGui::PushItemWidth(60.0f * core::GetWidgetWidthCoefficient());
                    if (ImGui::InputFloat("##NormalMagnitude", &core::SETTINGS.NormalMagnitude))
                    {
                        core::SETTINGS.NormalMagnitude = core::SETTINGS.NormalMagnitude > 0.0f ? core::SETTINGS.NormalMagnitude : 0.1f;
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        // Graphics
        if (ImGui::TreeNode("Graphics"))
        {
            // Full screen
            {
                // TODO
            }
            // Gamma
            {
                ImGui::CenterAlignWidget("Gamma", 150.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Gamma");
                ImGui::PushItemWidth(150.0f * core::GetWidgetWidthCoefficient());
                ImGui::SliderFloat("##Gamma", &core::SETTINGS.Gamma, 0.0f, 3.0f);
                ImGui::PopItemWidth();
            }
            // Resolution
            {
                ImGui::CenterAlignWidget("Resolution", 200.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Resolution");
                ImGui::PushItemWidth(200.0f * core::GetWidgetWidthCoefficient());
                static int resolution;
                resolution = (int)(100.0f * core::SETTINGS.Resolution);
                if (ImGui::SliderInt("##Resolution", &resolution, SETTING_RES_MIN, SETTING_RES_MAX))
                {
                    core::SETTINGS.Resolution = (float)resolution / 100.0f;
                    core::UpdateRenderRes();
                    m_Renderer.ChangeResolution();
                }
                ImGui::PopItemWidth();
            }
            // Anti-Alising
            {
                ImGui::CenterAlignWidget("Anti Alising", 100.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Anti Alising");
                ImGui::PushItemWidth(100.0f * core::GetWidgetWidthCoefficient());
                const char* aaOps[] = {
                    "Off",
                    "FXAA"
                };
                static int currentAA;
                currentAA = core::SETTINGS.AA;
                if (ImGui::Combo("##AntiAlising", &currentAA, aaOps, IM_ARRAYSIZE(aaOps)))
                {
                    core::SETTINGS.AA = (Anti_Alising)currentAA;
                }
                ImGui::PopItemWidth();
            }
            ImGui::TreePop();
        }
        // Reset all settings to default
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ImGui::COLOR_WARNING_SYTLE));
        ImGui::CenterAlignWidget("Reset");
        if (ImGui::Button("Reset"))
        {
            core::SETTINGS.Sensitivity = 1.0f;
            core::SETTINGS.EnableController = false;
            core::SETTINGS.DeadZone = glm::vec2(0.05f, 0.05f);
            core::SETTINGS.DrawMode = WIREFRAME;
            core::SETTINGS.ShowNormal = false;
            core::SETTINGS.NormalColor = glm::vec3(1.0f);
            core::SETTINGS.NormalMagnitude = 1.0f;
            core::SETTINGS.PP = NO_PP;
            m_Renderer.ChangePostProcess();
            core::SETTINGS.Gamma = 2.2f;
            core::SETTINGS.Resolution = 1.0f;
            m_Renderer.ChangeResolution();
            core::SETTINGS.AA = NO_AA;
        }
        ImGui::PopStyleColor(1);
        ImGui::End();
    }
}

void Application::DrawWarningWindow()
{
    if (core::IS_WARNING_OPENED)
    {
        ImGui::OpenPopup("Warning");
    }
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    windowFlags |= ImGuiWindowFlags_NoScrollbar;
    windowFlags |= ImGuiWindowFlags_NoMove;
    if (ImGui::BeginPopupModal("Warning", &core::IS_WARNING_OPENED, windowFlags)) 
    {
        // Calculate display and window size to center align warning window
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImVec2 displaySize = io.DisplaySize;
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImGui::SetWindowPos(ImVec2((displaySize.x - windowSize.x) / 2.0f, (displaySize.y - windowSize.y) / 2.0f));
        ImGui::CenterAlignWidget(core::POPUP_MSG.c_str());
        ImGui::Text(core::POPUP_MSG.c_str());
        ImGui::EndPopup();
    }
}

void Application::DrawLoadingWindow()
{
    if (core::FILEPATH_BUFFER != "")
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
        std::string filename = core::GetFileName(core::FILEPATH_BUFFER);
        ImGui::CenterAlignWidget("Loading " + filename);
        ImGui::Text(("Loading " + filename).c_str());
        ImGui::CenterAlignWidget("Please wait patiently :)");
        ImGui::Text("Please wait patiently :)");
        ImGui::End();
    }
}

void Application::DrawAboutRenderBoyWindow()
{
    if (core::IS_ABOUT_OPENED)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoScrollbar;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("About RenderBoy", &core::IS_ABOUT_OPENED, windowFlags);
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
    core::SETTINGS.Width = width;
    core::SETTINGS.Height = height;
    core::UpdateRenderRes();
}

void Application::KeyboardInput()
{    
    if (!core::IS_SETTINGS_OPENED && !ImGui::IsAnyItemActive())
    {
        // Camera Movement
        glm::vec3 move = glm::vec4(0.0f);
        if (ImGui::IsKeyDown(ImGuiKey_W)) // Forward
        {
            glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(0.0f, 0.0f, -1.0f));
            direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
            move = direction * m_Scene.GetCamera().GetMoveSpeed();
            move += m_Scene.GetCamera().GetPosition();
            m_Scene.GetCamera().SetPosition(move);
        }
        if (ImGui::IsKeyDown(ImGuiKey_S)) // Backward
        {
            glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(0.0f, 0.0f, 1.0f));
            direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
            move = direction * m_Scene.GetCamera().GetMoveSpeed();
            move += m_Scene.GetCamera().GetPosition();
            m_Scene.GetCamera().SetPosition(move);
        }
        if (ImGui::IsKeyDown(ImGuiKey_A)) // Left
        {
            glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
            direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
            move = direction * m_Scene.GetCamera().GetMoveSpeed();
            move += m_Scene.GetCamera().GetPosition();
            m_Scene.GetCamera().SetPosition(move);
        }
        if (ImGui::IsKeyDown(ImGuiKey_D)) // Right
        {
            glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
            direction = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
            move = direction * m_Scene.GetCamera().GetMoveSpeed();
            move += m_Scene.GetCamera().GetPosition();
            m_Scene.GetCamera().SetPosition(move);
        }
        if (ImGui::IsKeyDown(ImGuiKey_Space)) // Up
        {
            move = glm::vec3(0.0f, 1.0f, 0.0f) * m_Scene.GetCamera().GetMoveSpeed();
            move += m_Scene.GetCamera().GetPosition();
            m_Scene.GetCamera().SetPosition(move);
        }
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) // Down
        {
            move = glm::vec3(0.0f, -1.0f, 0.0f) * m_Scene.GetCamera().GetMoveSpeed();
            move += m_Scene.GetCamera().GetPosition();
            m_Scene.GetCamera().SetPosition(move);
        }
        // Camera Movement

        // Camera Rotation
        if (ImGui::IsKeyDown(ImGuiKey_Q)) // Roll left
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.z -= m_Scene.GetCamera().GetRotateSpeed();
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        if (ImGui::IsKeyDown(ImGuiKey_E)) // Roll right
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.z += m_Scene.GetCamera().GetRotateSpeed();
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        if (ImGui::IsKeyDown(ImGuiKey_Q) && ImGui::IsKeyDown(ImGuiKey_E))
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.z = 0.0f;
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) // Tilt up
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.x += m_Scene.GetCamera().GetRotateSpeed();
            rotate.x = rotate.x > 89.0f ? 89.0f : rotate.x;
            rotate.x = rotate.x < -89.0f ? -89.0f : rotate.x;
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) // Tilt down
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.x -= m_Scene.GetCamera().GetRotateSpeed();
            rotate.x = rotate.x > 89.0f ? 89.0f : rotate.x;
            rotate.x = rotate.x < -89.0f ? -89.0f : rotate.x;
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) // Turn left
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.y += m_Scene.GetCamera().GetRotateSpeed();
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) // Turn right
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.y -= m_Scene.GetCamera().GetRotateSpeed();
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
        // Camera Rotation
    }

    // Save Screenshot
    if (ImGui::IsKeyReleased(ImGuiKey_F5) && m_Launched)
    {
        if (!core::IS_WARNING_OPENED)
        {
            m_Renderer.SaveScreenShot(m_Scene);
        }
    }
    // Save Screenshot

    // Hide UI
    if (ImGui::IsKeyReleased(ImGuiKey_F2) && m_Launched)
    {
        core::IS_UI_OPENED = !core::IS_UI_OPENED;
    }
}

void Application::MouseInput()
{
    // Camera Rotation
    if (!core::IS_SETTINGS_OPENED)
    {
        if (ImGui::GetIO().MouseDown[1] && (ImGui::GetIO().MousePos.x >= 0.0f && ImGui::GetIO().MousePos.y >= 0.0f))
        {
            glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
            rotate.y += m_Scene.GetCamera().GetRotateSpeed() * ImGui::GetIO().MouseDelta.x * core::SETTINGS.Sensitivity;
            rotate.x += m_Scene.GetCamera().GetRotateSpeed() * ImGui::GetIO().MouseDelta.y * core::SETTINGS.Sensitivity;
            rotate.x = rotate.x > 89.0f ? 89.0f : rotate.x;
            rotate.x = rotate.x < -89.0f ? -89.0f : rotate.x;
            m_Scene.GetCamera().SetEulerAngle(rotate);
        }
    }
    // Camera Rotation

    // Model Rotation
    if (!core::IS_SETTINGS_OPENED)
    {
        if (ImGui::GetIO().MouseDown[2] && (ImGui::GetIO().MousePos.x >= 0.0f && ImGui::GetIO().MousePos.y >= 0.0f))
        {
            if (core::currentModelScene)
            {
                glm::vec3 rotate = m_Scene.GetModels()[core::currentModelScene].GetEulerAngle(m_Scene.GetModels()[core::currentModelScene].GetCurrent());
                rotate.y += ImGui::GetIO().MouseDelta.x * core::SETTINGS.Sensitivity;
                rotate.x -= ImGui::GetIO().MouseDelta.y * core::SETTINGS.Sensitivity;
                m_Scene.GetModels()[core::currentModelScene].SetEulerAngle(rotate, m_Scene.GetModels()[core::currentModelScene].GetCurrent());
            }
        }
    }
    // Model Rotation
}

void Application::GamepadInput()
{
    GLFWgamepadstate state;
    if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state) && m_Launched)
    {
        if (!core::IS_SETTINGS_OPENED)
        {
            glm::vec3 move = glm::vec4(0.0f);
            // Camera movement
            if (abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]) > core::SETTINGS.DeadZone.x || abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]) > core::SETTINGS.DeadZone.x)
            {
                glm::vec3 direction = m_Scene.GetCamera().GetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
                direction = glm::normalize(direction);
                if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > 0.0f)
                {
                    move = direction * m_Scene.GetCamera().GetMoveSpeed() * (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] - core::SETTINGS.DeadZone.x);
                }
                else
                {
                    move = direction * m_Scene.GetCamera().GetMoveSpeed() * (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] + core::SETTINGS.DeadZone.x);
                }
                move -= m_Scene.GetCamera().GetPosition();
                m_Scene.GetCamera().SetPosition(move);
                direction = m_Scene.GetCamera().GetDirection(glm::vec3(0.0f, 0.0f, -1.0f));
                direction = glm::normalize(direction);
                if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > 0.0f)
                {
                    move = direction * m_Scene.GetCamera().GetMoveSpeed() * -(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] - core::SETTINGS.DeadZone.x);
                }
                else
                {
                    move = direction * m_Scene.GetCamera().GetMoveSpeed() * -(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] + core::SETTINGS.DeadZone.x);
                }
                move -= m_Scene.GetCamera().GetPosition();
                m_Scene.GetCamera().SetPosition(move);
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB])
            {
                move = glm::vec3(0.0f, 1.0f, 0.0f) * m_Scene.GetCamera().GetMoveSpeed();
                move += m_Scene.GetCamera().GetPosition();
                m_Scene.GetCamera().SetPosition(move);
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB])
            {
                move = glm::vec3(0.0f, -1.0f, 0.0f) * m_Scene.GetCamera().GetMoveSpeed();
                move += m_Scene.GetCamera().GetPosition();
                m_Scene.GetCamera().SetPosition(move);
            }
            // Camera movement

            // Camera rotation
            if (abs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]) > core::SETTINGS.DeadZone.y || abs(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]) > core::SETTINGS.DeadZone.y)
            {
                glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
                if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > 0.0f)
                {
                    rotate.y += m_Scene.GetCamera().GetRotateSpeed() * -(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] - core::SETTINGS.DeadZone.y);
                }
                else
                {
                    rotate.y += m_Scene.GetCamera().GetRotateSpeed() * -(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] + core::SETTINGS.DeadZone.y);
                }
                m_Scene.GetCamera().SetEulerAngle(rotate);
                rotate = m_Scene.GetCamera().GetEulerAngle();
                if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > 0.0f)
                {
                    rotate.x += m_Scene.GetCamera().GetRotateSpeed() * -(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] - core::SETTINGS.DeadZone.y);
                }
                else
                {
                    rotate.x += m_Scene.GetCamera().GetRotateSpeed() * -(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] + core::SETTINGS.DeadZone.y);
                }
                rotate.x = rotate.x > 89.0f ? 89.0f : rotate.x;
                rotate.x = rotate.x < -89.0f ? -89.0f : rotate.x;
                m_Scene.GetCamera().SetEulerAngle(rotate);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > -1.0f)
            {
                glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
                rotate.z -= m_Scene.GetCamera().GetRotateSpeed() * (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) * 0.5f;
                m_Scene.GetCamera().SetEulerAngle(rotate);
            }
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > -1.0f)
            {
                glm::vec3 rotate = m_Scene.GetCamera().GetEulerAngle();
                rotate.z += m_Scene.GetCamera().GetRotateSpeed() * (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) * 0.5f;
                m_Scene.GetCamera().SetEulerAngle(rotate);
            }
            // Camera rotation

            // Reset Camera
            if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] && state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
            {
                m_Scene.GetCamera().SetRoll(0.0f);
            }
            if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] && state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB])
            {
                m_Scene.GetCamera().SetEulerAngle(glm::vec3(0.0f));
                m_Scene.GetCamera().SetPosition(glm::vec3(0.0f));
            }
            // Reset Camera
        }

        static bool isGamepadStartPressed;
        if (state.buttons[GLFW_GAMEPAD_BUTTON_START])
        {
            isGamepadStartPressed = true;
        }
        if (isGamepadStartPressed != state.buttons[GLFW_GAMEPAD_BUTTON_START])
        {
            // Save Screenshot
            isGamepadStartPressed = false;
            m_Renderer.SaveScreenShot(m_Scene);
        }

        static bool isGamepadDpadUpPressed;
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
        {
            isGamepadDpadUpPressed = true;
        }
        if (isGamepadDpadUpPressed != state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
        {
            // Switch post process effect
            isGamepadDpadUpPressed = false;
            core::SETTINGS.PP = (Post_Process)((core::SETTINGS.PP + 1) % POST_PROCESS_COUNT);
            m_Renderer.ChangePostProcess();
        }

        static bool isGamepadDpadDownPressed;
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
        {
            isGamepadDpadDownPressed = true;
        }
        if (isGamepadDpadDownPressed != state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
        {
            // Switch post process effect
            isGamepadDpadDownPressed = false;
            core::SETTINGS.PP = (Post_Process)((core::SETTINGS.PP - 1 < 0 ? POST_PROCESS_COUNT - 1 : core::SETTINGS.PP - 1) % POST_PROCESS_COUNT);
            m_Renderer.ChangePostProcess();
        }

        static bool isGamepadDpadLeftPressed;
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
        {
            isGamepadDpadLeftPressed = true;
        }
        if (isGamepadDpadLeftPressed != state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
        {
            // Switch draw mode
            isGamepadDpadLeftPressed = false;
            core::SETTINGS.DrawMode = (Draw_Mode)((core::SETTINGS.DrawMode - 1 < 0 ? DRAW_MODE_COUNT - 1 : core::SETTINGS.DrawMode - 1) % DRAW_MODE_COUNT);
            m_Scene.GetData().ChangeDrawMode();
        }

        static bool isGamepadDpadRightPressed;
        if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
        {
            isGamepadDpadRightPressed = true;
        }
        if (isGamepadDpadRightPressed != state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
        {
            // Switch draw mode
            isGamepadDpadRightPressed = false;
            core::SETTINGS.DrawMode = (Draw_Mode)((core::SETTINGS.DrawMode + 1) % DRAW_MODE_COUNT);
            m_Scene.GetData().ChangeDrawMode();
        }

        if (core::IS_WARNING_OPENED && state.buttons[GLFW_GAMEPAD_BUTTON_B])
        {
            core::IS_WARNING_OPENED = false;
        }

        if (state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE])
        {
            spdlog::warn("test");
        }
    }
}