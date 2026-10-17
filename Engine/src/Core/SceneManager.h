//
// Created by Kreiseljustus on 10/17/2026.
//

#ifndef BLOCKGAME_SCENEMANAGER_H
#define BLOCKGAME_SCENEMANAGER_H
#include "ECS/Scene.h"
#include "Rendering/Renderer.h"

namespace Engine::Core {
    class SceneManager {
        public:
        template<typename T, typename... Args>
        void LoadScene(Args&&... args) {
            static_assert(std::is_base_of_v<Scene, T>, "Not a Scene!");
            m_Pending = std::make_unique<T>(std::forward<Args>(args)...);
        }

        void OnUpdate(float dt);
        void OnRender(Rendering::Renderer& renderer) const;

        bool HasActiveScene() const {return m_Active != nullptr;}
        Scene& GetActiveScene() const;
        private:
        std::unique_ptr<Scene> m_Active;
        std::unique_ptr<Scene> m_Pending;
    };
}


#endif //BLOCKGAME_SCENEMANAGER_H
