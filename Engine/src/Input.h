//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_INPUT_H
#define BLOCKGAME_INPUT_H


#include <unordered_map>
#include <glm/glm.hpp>

#include "Core/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Engine {
    class Input {
    public:
        void OnEvent(Core::Event& event);

        bool IsKeyDown(const int key) const {
            const auto it = m_KeyState.find(key);
            return it != m_KeyState.end() && it->second;
        }

        bool IsMouseButtonDown(const int button) const {
            const auto it = m_MouseButtonState.find(button);
            return it != m_MouseButtonState.end() && it->second;
        }

        glm::vec2 ConsumeMouseDelta() {
            const glm::vec2 delta = m_MouseDelta;
            m_MouseDelta = {0,0};
            return delta;
        }

    private:
        void OnKeyPressed(const Events::KeyPressedEvent& e);
        void OnKeyReleased(const Events::KeyReleasedEvent& e);
        void OnMouseButtonPressed(const Events::MouseButtonPressedEvent& e);
        void OnMouseButtonReleased(const Events::MouseButtonReleasedEvent& e);
        void OnMouseMoved(const Events::MouseMovedEvent& e);
    private:
        std::unordered_map<int, bool> m_KeyState;
        std::unordered_map<int,bool> m_MouseButtonState;

        glm::vec2 m_LastMousePos = {0,0};
        glm::vec2 m_MouseDelta = {0,0};
        bool m_FirstMouseSample = true;
    };
}

#endif //BLOCKGAME_INPUT_H