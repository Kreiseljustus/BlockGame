//
// Created by Kreiseljustus on 10/14/2026.
//

#ifndef BLOCKGAME_GAMEOBJECT_H
#define BLOCKGAME_GAMEOBJECT_H
#include "Scene.h"
#include "../headerLibs/entt.hpp"

namespace Engine {
    class GameObject {
    public:
        GameObject() = default;
        GameObject(entt::entity e, Scene* scene) : m_Entity(e), m_Scene(scene) {}

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            assert(!HasComponent<T>() && "GameObject already has this component");
            return m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent() {
            assert(HasComponent<T>() && "GameObject doesnt have that component");
            return m_Scene->m_Registry.get<T>(m_Entity);
        }

        template<typename T>
        bool HasComponent() const {
            return m_Scene->m_Registry.all_of<T>(m_Entity);
        }

        template<typename T>
        void RemoveComponent() const {
            assert(HasComponent<T>());
            m_Scene->m_Registry.remove<T>(m_Entity);
        }

        bool IsValid() const {return m_Scene && m_Scene->m_Registry.valid(m_Entity);}
        explicit operator bool() const {return IsValid();}
        bool operator==(const GameObject& o) const {return m_Entity == o.m_Entity && m_Scene == o.m_Scene;}
    private:
        entt::entity m_Entity{entt::null};
        Scene* m_Scene = nullptr;

        friend class Scene;
    };
}
#endif //BLOCKGAME_GAMEOBJECT_H
