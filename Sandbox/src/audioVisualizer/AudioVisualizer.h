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
    ~AudioVisualizer();
    void OnRenderImGui();
protected:
    void OnSceneUpdate() override;
    void OnLoad() override;
private:
    Engine::GameObject m_Music;
    Analyzer m_Analyzer;
};


#endif //BLOCKGAME_AUDIOVISUALIZER_H
