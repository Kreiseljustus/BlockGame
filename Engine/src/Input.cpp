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

void Input::handleMouseButton(int button, int action, int mods) {
    if (action == GLFW_PRESS) m_MouseButtonState[button] = true;
    else if (action == GLFW_RELEASE) m_MouseButtonState[button] = false;
}

void Input::handleCursorPos(double x, double y) {
    glm::vec2 pos = {static_cast<float>(x), static_cast<float>(y)};
    if (m_FirstMouseSample) {
        m_LastMousePos = pos;
        m_FirstMouseSample = false;
        return;
    }
    m_MouseDelta += (pos - m_LastMousePos);
    m_LastMousePos = pos;
}
