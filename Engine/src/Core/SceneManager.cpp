//
// Created by Kreiseljustus on 10/17/2026.
//

#include "SceneManager.h"

#include <algorithm>


namespace Engine::Core {

    void SceneManager::OnUpdate(float dt) {
        if (m_Pending) {
            if (m_Active) m_Active->OnUnload();
            m_Active = std::move(m_Pending);
            m_Active->OnLoad();
        }
        if (m_Active) m_Active->OnUpdate(dt);
    }

    void SceneManager::OnRender(Rendering::Renderer& renderer) const {
        if (m_Active) m_Active->OnRender(renderer);
    }

    Scene& SceneManager::GetActiveScene() const {
        assert(m_Active && "SceneManager: no active scene");
        return *m_Active;
    }

}
