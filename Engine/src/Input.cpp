//
// Created by crisel on 6/23/26.
//

#include "Input.h"

using namespace Engine;

void Input::handleKey(const int key, int scancode, const int action, int mods) {
    if (action == GLFW_PRESS) {
        m_KeyState[key] = true;
    } else if (action == GLFW_RELEASE) {
        m_KeyState[key] = false;
    }
}

void Input::handleResize(const int width, const int height) {
    glViewport(0,0, width, height);
}
