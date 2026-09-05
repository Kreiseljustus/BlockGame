//
// Created by Kreiseljustus on 8/17/2026.
//

#ifndef BLOCKGAME_EVENT_H
#define BLOCKGAME_EVENT_H

#define EN_EVENT_CLASS_TYPE(type) \
    static Core::EventType GetStaticType() {return Core::EventType::type;} \
    Core::EventType GetType() const override {return GetStaticType();}

namespace Engine::Core {
    enum class EventType {
        WindowResize, WindowClose,
        KeyPressed, KeyReleased,
        MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled
    };

    class Event {
    public:
        virtual ~Event() = default;
        virtual EventType GetType() const = 0;
        bool Handled = false;
    };

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& event) : m_Event(event) {}

        template<typename T, typename Func>
        bool Dispatch(Func func) {
            if (m_Event.GetType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };
}

#endif //BLOCKGAME_EVENT_H
