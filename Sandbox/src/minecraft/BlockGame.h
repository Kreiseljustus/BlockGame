//
// Created by Kreiseljustus on 9/6/2026.
//

#ifndef BLOCKGAME_BLOCKGAME_H
#define BLOCKGAME_BLOCKGAME_H

#include <Core/Layer.h>

#include "ChunkManager.h"
#include "Window.h"
#include "Rendering/RenderData.h"
#include "Rendering/Renderer.h"

using namespace Engine;
using namespace Engine::Core;
using namespace Engine::Rendering;


std::vector<Transform> fillUp(Transform t);

class BlockGame : public Layer {
public:
    BlockGame();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(float deltaTime) override;
    void OnEvent(Event &event) override;

private:
    Renderer renderer;
    Camera camera;
    Window* window;

    TextureHandle tHandle;
    ShaderHandle test_Shader_handle;

    ChunkManager chunkManager;

    Input* input;

    const siv::PerlinNoise::seed_type seed = 12345;
    const siv::PerlinNoise perlin{seed};
};

#endif //BLOCKGAME_BLOCKGAME_H
