//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_INPUT_H
#define BLOCKGAME_INPUT_H
#include <iostream>

#include "GLFW/glfw3.h"

class Input {
public:
    void handleKey(int key, int scancode, int action, int mods);
    void handleResize(int width, int height);

    static void keyCallback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
        auto* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
        if (!self) {std::cout << "No input setup for window " << window << std::endl; return; }
        self->handleKey(key,scancode,action,mods);
    }

    static void resizeCallback(GLFWwindow* window, const int width, const int height) {
        auto* self = static_cast<Input*>(glfwGetWindowUserPointer(window));
        if (!self) {std::cout << "No input setup for window " << window << std::endl; return;}
        self->handleResize(width, height);
    }
};

#endif //BLOCKGAME_INPUT_H