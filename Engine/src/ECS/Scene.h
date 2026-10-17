//
// Created by Kreiseljustus on 10/14/2026.
//

#ifndef BLOCKGAME_SCENE_H
#define BLOCKGAME_SCENE_H

#include "ISystem.h"
#include "../../headerLibs/entt.hpp"

#include "../Rendering/Renderer.h"
namespace Engine {
    namespace Core {
        class SceneManager;
    }

    class GameObject;

    class Scene {
    public:
        Scene() = default;
        virtual ~Scene() = default;
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        GameObject CreateGameObject(const std::string& name = "new GameObject");
        void DestroyGameObject(GameObject o);

        void OnUpdate(float dt);
        void OnRender(Rendering::Renderer& renderer);

        template<typename T, typename... Args>
        T& AddSystem(Args&&... args) {
            auto sys = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *sys;
            m_Systems.push_back(std::move(sys));
            ref.OnAttach(*this);
            return ref;
        }

        entt::registry&   Registry() { return m_Registry; }
        entt::dispatcher& Events()   { return m_Dispatcher; }

    protected:
        virtual void OnLoad() {}
        virtual void OnUnload() {}

    private:
        entt::registry m_Registry;
        entt::dispatcher m_Dispatcher;
        std::vector<std::unique_ptr<ISystem>> m_Systems;

        friend class GameObject;
        friend class Core::SceneManager;
    };
}

#endif //BLOCKGAME_SCENE_H