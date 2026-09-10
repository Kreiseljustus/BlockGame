//
// Created by Kreiseljustus on 8/11/2026.
//

#ifndef BLOCKGAME_APPLICATION_H
#define BLOCKGAME_APPLICATION_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Window.h"

#include "Core/Layer.h"
#include "Event.h"
#include "Events/WindowEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace Engine::Core {

    struct ApplicationSpecification {
        std::string Name = "New Application";
        std::string WorkingDirectory;
    };

    class Application {
    public:
        Application(const ApplicationSpecification& specs);
        virtual ~Application();

        void PushLayer(Layer* layer);

        void OnEvent(Event& event);
        Input* GetInput() {return &input;}

        [[nodiscard]] Window& GetWindow() const;
        bool OnWindowResize(Events::WindowResizeEvent& e);

        void Close();

        static Application& Get() {return *s_Instance;}
        void Run();
    private:

        //DONT MOVE
        Input input;

        ApplicationSpecification m_Specs;
        std::unique_ptr<Window> m_Window;
        std::vector<Layer*> m_LayerStack;
       // ImGuiLayer m_ImGuiLayer;

        bool m_Running = true;
        bool m_Minimized = false;

        double m_LastFrameTime = 0.0f;
        static Application* s_Instance;
    };
}


#endif //BLOCKGAME_APPLICATION_H
