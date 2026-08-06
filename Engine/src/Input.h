//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_INPUT_H
#define BLOCKGAME_INPUT_H
#include <iostream>
#include <unordered_map>

#include "GLFW/glfw3.h"

namespace Engine {
    class Input {
    public:
        void handleKey(int key, int scancode, int action, int mods);

        static void handleResize(int width, int height);

        bool IsKeyDown(const int key) const {
            const auto it = m_KeyState.find(key);
            return it != m_KeyState.end() && it->second;
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

    private:
        std::unordered_map<int, bool> m_KeyState;
    };
}

#endif //BLOCKGAME_INPUT_H