//
// Created by Kreiseljustus on 8/11/2026.
//

#include "Application.h"

#include <filesystem>
#include <ranges>

#include "Events/WindowEvent.h"
#include "Rendering/Backends/OpenGLBackend.h"

using namespace Engine::Core;

Application* Application::s_Instance = nullptr;

Application::Application(const ApplicationSpecification& specs) : m_Specs(specs){
    s_Instance = this;

    if (!m_Specs.WorkingDirectory.empty()) {
        std::filesystem::current_path(m_Specs.WorkingDirectory);
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
    }

    m_Window = std::make_unique<Window>(WindowProperties{.Title = m_Specs.Name});
    m_Window->SetEventCallback([this](Event& e) {OnEvent(e);});
    m_Window->create();

    m_ImGuiLayer = new ImGuiLayer();
    PushLayer(m_ImGuiLayer);

    m_Renderer = std::make_unique<Rendering::Renderer>(std::make_unique<Rendering::OpenGLBackend>());
}

Application::~Application() {
    glfwTerminate();
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.push_back(layer);
    layer->OnAttach();
}

void Application::Close() {
    m_Running = false;

    glfwTerminate();
    exit(0);
}

void Application::OnEvent(Event& event) {
    input.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Events::WindowResizeEvent>([this](Events::WindowResizeEvent& resize) {return OnWindowResize(resize);});
    dispatcher.Dispatch<Events::WindowCloseEvent>([this](Events::WindowCloseEvent&) {Close(); return true;});

    for (const auto& it : std::views::reverse(m_LayerStack)) {
        if (event.Handled) break;
        it->OnEvent(event);
    }
}

Engine::Window & Application::GetWindow() const {
    return *m_Window;
}

bool Application::OnWindowResize(Events::WindowResizeEvent &e) {

    if (e.Width == 0 || e.Height == 0) {
        m_Minimized = true;
        return false;
    } else {
        m_Minimized = false;
        glViewport(0,0, e.Width, e.Height);
    }
    return true;
}

void Application::Run() {
    while (m_Running) {
        const double now = glfwGetTime();
        const auto deltaTime = static_cast<float>(now - m_LastFrameTime);
        m_LastFrameTime = now;

        glfwPollEvents();

        if (glfwWindowShouldClose(m_Window->getWindow())) Close();

        if (!m_Minimized) {
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(deltaTime);
            }

            m_SceneManager.OnUpdate(deltaTime);

            const auto fb = m_Window->getFrameBufferSize();
            const float aspect = fb.y > 0 ? float(fb.x) / float(fb.y) : 1.0f;

            std::optional<Rendering::Camera> sceneCam;
            const Rendering::Camera* camera = m_CameraOverride;

            if (!camera && m_SceneManager.HasActiveScene()) {
                sceneCam = m_SceneManager.GetActiveScene().GetPrimaryCamera(aspect);
                if (sceneCam) camera = &*sceneCam;
            }

            //TODO: When camera is null frameBuffers not cleared :c

            if (camera) {
                m_Renderer->Begin(*camera);
                m_SceneManager.OnRender(*m_Renderer);
                for (Layer* layer : m_LayerStack) {
                    //TODO: Might be able to remove
                    layer->OnRender();
                }
                m_Renderer->End();
            }

            ImGuiLayer::Begin();
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            ImGuiLayer::End();

            glfwSwapBuffers(m_Window->getWindow());
        }
    }
}
