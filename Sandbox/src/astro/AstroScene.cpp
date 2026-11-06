//
// Created by Kreiseljustus on 10/21/2026.
//

#include "AstroScene.h"

#include <iostream>

#include "ECS/BasicComponents.h"

#include "imgui.h"
#include "ResourceManager.h"
#include "Rendering/PrimitiveProvider.h"

AstroScene::AstroScene(Engine::Rendering::Renderer& r) : Scene(r) {
    renderer = &r;
}

AstroScene::~AstroScene() = default;

void AstroScene::OnRenderImGui() {

}

void AstroScene::OnSceneUpdate(float dt) {

}

void AstroScene::OnLoad() {
    m_Cube = renderer->CreateMesh(GetUnitCubeCCW());
    Engine::Rendering::ShaderHandle a = Engine::ResourceManager::LoadShader("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl", "basic");
    Engine::Rendering::TextureParameters c = Engine::Rendering::TextureParameters();
    Engine::ResourceManager::LoadTexture(c, "assets/textures/dirt.png", "b");
    Engine::Rendering::TextureHandle b = renderer->CreateTexture(Engine::ResourceManager::LoadTexture(c, "assets/textures/dirt.png", "b"));
    m_Material = {a,b,false};

    m_Music = CreateGameObject();
    auto& audioComponent = m_Music.AddComponent<Engine::Components::AudioSourceComponent>();
    bool sucess = audioComponent.Load("assets/audio/test.mp3");
    std::cout << "Successfully loaded? " << sucess << std::endl;
    audioComponent.SetVolume(0.1f);
    audioComponent.SetLooping(true);
    audioComponent.Play();

    m_Camera = CreateGameObject();
    auto& cameraComponent = m_Camera.AddComponent<Engine::Components::CameraComponent>();
    cameraComponent.primary = true;
    cameraComponent.fov = 90;

    m_Camera.GetComponent<Engine::Rendering::Transform>().position = {0,2, 20};
}
