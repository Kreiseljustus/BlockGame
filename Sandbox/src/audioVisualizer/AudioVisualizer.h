//
// Created by Kreiseljustus on 10/21/2026.
//

#ifndef BLOCKGAME_AUDIOVISUALIZER_H
#define BLOCKGAME_AUDIOVISUALIZER_H
#include "Analyzer.h"
#include "ECS/GameObject.h"
#include "ECS/Scene.h"


class AudioVisualizer : public Engine::Scene {
public:
    AudioVisualizer(Engine::Rendering::Renderer& r);
    ~AudioVisualizer();
    void OnRenderImGui();
protected:
    void OnSceneUpdate() override;
    //TODO: remove this Renderer for resource creation
    void OnLoad() override;
private:
    Engine::GameObject m_Music;
    Analyzer m_Analyzer;

    Engine::Rendering::MeshHandle m_Cube;
    Engine::Rendering::Material m_Material;
    std::vector<Engine::GameObject> m_AudioBars;
    Engine::Rendering::Renderer* renderer;
};


#endif //BLOCKGAME_AUDIOVISUALIZER_H
