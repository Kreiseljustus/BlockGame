//
// Created by Kreiseljustus on 8/17/2026.
//

#ifndef BLOCKGAME_KEYEVENT_H
#define BLOCKGAME_KEYEVENT_H
#include "Core/Event.h"

namespace Engine::Events {
    class KeyPressedEvent : public Core::Event {
    public:
        explicit KeyPressedEvent(const int key) : Key(key) {}
        EN_EVENT_CLASS_TYPE(KeyPressed);
        int Key;
    };

    class KeyReleasedEvent : public Core::Event {
    public:
        explicit KeyReleasedEvent(const int key) : Key(key) {}
        EN_EVENT_CLASS_TYPE(KeyReleased);
        int Key;
    };
}

#endif //BLOCKGAME_KEYEVENT_H
