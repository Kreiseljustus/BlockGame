//
// Created by Kreiseljustus on 9/6/2026.
//

#include "BlockGame.h"

#include <stb_image.h>

#include "ResourceManager.h"
#include "Core/Application.h"
#include "Rendering/Backends/OpenGLBackend.h"


std::vector<Transform> fillUp(Transform t) {
    constexpr int worldBottom= -64;

    std::vector<Transform> fill;

    for (int y = worldBottom; y < t.position.y; ++y) {
        Transform currPos = t;
        currPos.position.y = y;
        fill.push_back(currPos);
    }

    return fill;
}

BlockGame::BlockGame() : renderer(std::make_unique<Rendering::OpenGLBackend>()), window(Application::Get().GetWindow()) {
    input = Application::Get().GetInput();
}

void BlockGame::OnAttach() {
    ResourceManager::LoadShader("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl", "basic");
    test_Shader_handle = ResourceManager::GetShader("basic");

    Rendering::TextureParameters tParams;
    int channels;
    tParams.imageData = stbi_load("assets/textures/dirt.png", &tParams.width, &tParams.height, &channels, 4);

    tHandle = renderer.CreateTexture(tParams);

    camera.projection = ProjectionType::Perspective;
    camera.position = {0,0,5};
    camera.orthoSize = 3.0f;
    camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

    glfwSwapInterval(0);
}

void BlockGame::OnDetach() {
    Layer::OnDetach();
}

void BlockGame::OnUpdate(const float deltaTime) {
    std::string title = "Block Game ";
    title.append(std::to_string(1.0f/deltaTime));
    glfwSetWindowTitle(window.getWindow(), title.c_str());


    camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

    const float lookSensitivity = 0.15f;
    float camSpeed = 6.0f;

    if (input->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) camSpeed *= 2;

    if (input->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glm::vec2 mouseDelta = input->ConsumeMouseDelta();
        camera.rotation.y -= mouseDelta.x * lookSensitivity; // yaw
        camera.rotation.x -= mouseDelta.y * lookSensitivity; // pitch
        camera.rotation.x = std::clamp(camera.rotation.x, -89.0f, 89.0f);

        float yawRad = glm::radians(camera.rotation.y);
        float pitchRad = glm::radians(camera.rotation.x);

        glm::vec3 forward = {
            -sinf(yawRad) * cosf(pitchRad),
            sinf(pitchRad),
            -cosf(yawRad) * cosf(pitchRad)
        };
        glm::vec3 right = {cosf(yawRad), 0, -sinf(yawRad)};

        if (input->IsKeyDown(GLFW_KEY_W)) camera.position += forward * camSpeed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_S)) camera.position -= forward * camSpeed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_A)) camera.position -= right * camSpeed * deltaTime;
        if (input->IsKeyDown(GLFW_KEY_D)) camera.position += right * camSpeed * deltaTime;
    } else {
        glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        input->ConsumeMouseDelta();
    }

    chunkManager.Update(camera.position, renderer, perlin);

    renderer.Begin(camera);
    

    chunkManager.Render(renderer, test_Shader_handle, tHandle);

    renderer.End();
}

void BlockGame::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Events::WindowResizeEvent>([this](Events::WindowResizeEvent& resize){return OnResize(resize);});
}

bool BlockGame::OnResize(Events::WindowResizeEvent& e) {
    camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);
    return true;
}
