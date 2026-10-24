//
// Created by Kreiseljustus on 10/14/2026.
//

#include "Scene.h"

#include "BasicComponents.h"
#include "GameObject.h"
#include "../Rendering/RenderData.h"

namespace Engine {
    Scene::Scene(Rendering::Renderer &) {
    }

    GameObject Scene::CreateGameObject(const std::string &name) {
        GameObject go{m_Registry.create(), this};
        go.AddComponent<Components::TagComponent>(name);
        go.AddComponent<Rendering::Transform>();
        return go;
    }

    void Scene::DestroyGameObject(GameObject o) {
        if (o.IsValid()) {
            m_Registry.destroy(o.m_Entity);
        }
    }

    void Scene::OnUpdate(float dt) {
        for (auto& sys : m_Systems) {
            sys->OnUpdate(*this, dt);
        }
        m_Dispatcher.update();

        //After all ecs systems ran the scenes get control, not sure if this is good or not will see
        OnSceneUpdate();
    }

    void Scene::OnRender(Rendering::Renderer &renderer) {
        auto view = m_Registry.view<Rendering::Transform, Components::MeshRendererComponent>();
        for (auto [entity, transform, mesh] : view.each()) {
            renderer.Submit(mesh.mesh, mesh.material, transform);
        }
    }
}