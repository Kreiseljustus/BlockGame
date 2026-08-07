//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_INPUT_H
#define BLOCKGAME_INPUT_H
#include <iostream>
#include <unordered_map>

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>

namespace Engine {
    class Input {
    public:
        void handleKey(int key, int scancode, int action, int mods);
        void handleMouseButton(int button, int action, int mods);
        void handleCursorPos(double x, double y);

        static void handleResize(int width, int height);

        bool IsKeyDown(const int key) const {
            const auto it = m_KeyState.find(key);
            return it != m_KeyState.end() && it->second;
        }

        bool IsMouseButtonDown(int button) const {
            auto it = m_MouseButtonState.find(button);
            return it != m_MouseButtonState.end() && it->second;
        }

        glm::vec2 ConsumeMouseDelta() {
            glm::vec2 delta = m_MouseDelta;
            m_MouseDelta = {0,0};
            return delta;
        }

        static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
            auto* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
            if (!self) { std::cout << "No input setup for window " << window << std::endl; return; }
            self->handleKey(key, scancode, action, mods);
        }

        static void resizeCallback(GLFWwindow* window, const int width, const int height) {
            auto* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
            if (!self) { std::cout << "No input setup for window " << window << std::endl; return; }
            handleResize(width, height);
        }

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
            if (!self) return;
            self->handleMouseButton(button,action,mods);
        }

        static void cursorPosCallback(GLFWwindow* window, double x, double y) {
            auto* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
            if (!self) return;
            self->handleCursorPos(x, y);
        }

    private:
        std::unordered_map<int, bool> m_KeyState;
        std::unordered_map<int,bool> m_MouseButtonState;

        glm::vec2 m_LastMousePos = {0,0};
        glm::vec2 m_MouseDelta = {0,0};
        bool m_FirstMouseSample = true;
    };
}

#endif //BLOCKGAME_INPUT_H