//
// Created by crisel on 6/23/26.
//

#include "Input.h"

using namespace Engine;

void Input::OnEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Events::KeyPressedEvent>([this](auto& e) { OnKeyPressed(e); return false; });
    dispatcher.Dispatch<Events::KeyReleasedEvent>([this](auto& e) { OnKeyReleased(e); return false; });
    dispatcher.Dispatch<Events::MouseButtonPressedEvent>([this](auto& e) { OnMouseButtonPressed(e); return false; });
    dispatcher.Dispatch<Events::MouseButtonReleasedEvent>([this](auto& e) { OnMouseButtonReleased(e); return false; });
    dispatcher.Dispatch<Events::MouseMovedEvent>([this](auto& e) { OnMouseMoved(e); return false; });
}

void Input::OnKeyPressed(const Events::KeyPressedEvent& e) { m_KeyState[e.Key] = true; }
void Input::OnKeyReleased(const Events::KeyReleasedEvent& e) { m_KeyState[e.Key] = false; }
void Input::OnMouseButtonPressed(const Events::MouseButtonPressedEvent& e) { m_MouseButtonState[e.Button] = true; }
void Input::OnMouseButtonReleased(const Events::MouseButtonReleasedEvent& e) { m_MouseButtonState[e.Button] = false; }

void Input::OnMouseMoved(const Events::MouseMovedEvent &e) {
    const glm::vec2 pos = {static_cast<float>(e.X), static_cast<float>(e.Y)};
    if (m_FirstMouseSample) {
        m_LastMousePos = pos;
        m_FirstMouseSample = false;
        return;
    }
    m_MouseDelta += (pos - m_LastMousePos);
    m_LastMousePos = pos;
}
