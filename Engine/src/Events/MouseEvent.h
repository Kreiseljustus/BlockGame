//
// Created by Kreiseljustus on 9/6/2026.
//

#ifndef BLOCKGAME_MOUSEEVENT_H
#define BLOCKGAME_MOUSEEVENT_H

#include <Core/Event.h>

namespace Engine::Events {
    class MouseMovedEvent : public Core::Event {
    public:
        MouseMovedEvent(const double x, const double y) : X(x), Y(y) {}
        EN_EVENT_CLASS_TYPE(MouseMoved);
        double X, Y;
    };

    class MouseScrolledEvent : public Core::Event {
    public:
        MouseScrolledEvent(const double xOffset, const double yOffset) : XOffset(xOffset), YOffset(yOffset) {}
        EN_EVENT_CLASS_TYPE(MouseScrolled);
        double XOffset, YOffset;
    };

    class MouseButtonPressedEvent : public Core::Event {
    public:
        explicit MouseButtonPressedEvent(const int button) : Button(button) {}
        EN_EVENT_CLASS_TYPE(MouseButtonPressed)
        int Button;
    };

    class MouseButtonReleasedEvent : public Core::Event {
    public:
        explicit MouseButtonReleasedEvent(const int button) : Button(button) {}
        EN_EVENT_CLASS_TYPE(MouseButtonReleased);
        int Button;
    };
}

#endif //BLOCKGAME_MOUSEEVENT_H
