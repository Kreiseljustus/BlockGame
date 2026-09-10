//
// Created by Kreiseljustus on 8/17/2026.
//

#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "Core/Application.h"
#include "imgui_impl_glfw.h"

Engine::ImGuiLayer::ImGuiLayer() = default;

void Engine::ImGuiLayer::OnAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    const Core::Application& app = Core::Application::Get();
    GLFWwindow* window = app.GetWindow().getWindow();

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Engine::ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Engine::ImGuiLayer::OnEvent(Core::Event& event) {
    Layer::OnEvent(event);
}

void Engine::ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Engine::ImGuiLayer::End() {
    ImGuiIO& io = ImGui::GetIO();
    Core::Application& app = Core::Application::Get();
    io.DisplaySize = ImVec2(static_cast<float>(app.GetWindow().getFrameBufferSize().x), static_cast<float>(app.GetWindow().getFrameBufferSize().y));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
