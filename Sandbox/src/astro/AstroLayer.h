//
// Created by Kreiseljustus on 10/21/2026.
//

#ifndef BLOCKGAME_ASTROLAYER_H
#define BLOCKGAME_ASTROLAYER_H

#include <Core/Layer.h>

#include "Core/Application.h"
#include "Rendering/Renderer.h"

using namespace Engine;
using namespace Engine::Core;
using namespace Engine::Rendering;

class AstroLayer : public Layer {
public:
    AstroLayer();

    void OnAttach() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
private:
    Application* m_App;
    Window m_Window;
    Input* input;
};


#endif //BLOCKGAME_ASTROLAYER_H
