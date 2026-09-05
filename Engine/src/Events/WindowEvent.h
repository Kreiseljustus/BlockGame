//
// Created by Kreiseljustus on 8/17/2026.
//

#ifndef BLOCKGAME_WINDOWRESIZE_H
#define BLOCKGAME_WINDOWRESIZE_H
#include "Core/Event.h"

namespace Engine::Events {
    class WindowResizeEvent : public Core::Event {
    public:
        WindowResizeEvent(const int width, const int height) : Width(width), Height(height) {}
        EN_EVENT_CLASS_TYPE(WindowResize);
        int Width, Height;
    };

    class WindowCloseEvent : public Core::Event {
    public:
        WindowCloseEvent() = default;
        EN_EVENT_CLASS_TYPE(WindowClose);
    };
}

#endif //BLOCKGAME_WINDOWRESIZE_H
