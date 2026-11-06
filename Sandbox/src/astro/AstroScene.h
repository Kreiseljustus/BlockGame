//
// Created by Kreiseljustus on 10/21/2026.
//

#ifndef BLOCKGAME_ASTROSCENE_H
#define BLOCKGAME_ASTROSCENE_H
#include "ECS/GameObject.h"
#include "ECS/Scene.h"


class AstroScene : public Engine::Scene {
public:
    AstroScene(Engine::Rendering::Renderer& r);
    ~AstroScene();
    void OnRenderImGui();
protected:
    void OnSceneUpdate(float dt) override;
    void OnLoad() override;
private:
    Engine::GameObject m_Music;
    Engine::GameObject m_Camera;

    Engine::Rendering::MeshHandle m_Cube;
    Engine::Rendering::Material m_Material;
    std::vector<Engine::GameObject> m_AudioBars;
    Engine::Rendering::Renderer* renderer;
};


#endif //BLOCKGAME_ASTROSCENE_H
