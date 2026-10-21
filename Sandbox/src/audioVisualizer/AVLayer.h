//
// Created by Kreiseljustus on 10/21/2026.
//

#ifndef BLOCKGAME_AVLAYER_H
#define BLOCKGAME_AVLAYER_H

#include <Core/Layer.h>

#include "Core/Application.h"
#include "Rendering/Renderer.h"

using namespace Engine;
using namespace Engine::Core;
using namespace Engine::Rendering;

class AVLayer : public Layer {
public:
    AVLayer();

    void OnAttach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
private:
    Application* m_App;
    Renderer m_Renderer;
};


#endif //BLOCKGAME_AVLAYER_H
