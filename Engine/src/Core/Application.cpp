//
// Created by Kreiseljustus on 8/11/2026.
//

#include "Application.h"

#include <filesystem>

#include "Events/WindowEvent.h"

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

    //m_ImGuiLayer = new ImGuiLayer();
    //PushLayer(m_ImGuiLayer);
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
}

void Application::OnEvent(Event& event) {
    input.OnEvent(event);

    EventDispatcher dispatcher(event);
    //dispatcher.Dispatch<Events::WindowResizeEvent>([this](Events::WindowResizeEvent& resize) {return OnWindowResize();});
    dispatcher.Dispatch<Events::WindowCloseEvent>([this](Events::WindowCloseEvent&) {Close(); return true;});

    for (auto it= m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        if (event.Handled) break;
        (*it)->OnEvent(event);
    }
}

Engine::Window & Application::GetWindow() const {
    return *m_Window;
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

            //m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            //m_ImGuiLayer->End();

            glfwSwapBuffers(m_Window->getWindow());
        }
    }
}
