#include <algorithm>
#include <iostream>

#include <Window.h>
#include <Input.h>

#include <ResourceManager.h>
#include <stb_image.h>
#include <Rendering/Renderer.h>
#include <Rendering/Backends/OpenGLBackend.h>

#include <Rendering/PrimitiveProvider.h>

#include "Core/Application.h"
#include "minecraft/ChunkManager.h"
#include "utils/PerlinNoise.hpp"

using namespace Engine::Rendering;
using namespace Engine;

#include "minecraft/Chunk.h"

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

int main(int arc, char* argv[]) {

	//Core::Application app = Core::Application(Core::ApplicationSpecification());
	//app.Run();


	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
	}

	volatile bool useOpenGL = true;

	WindowProperties props;
	props.Width = 800;
	props.Height = 600;
	props.Hidden = false;
	props.Title = "Block Game";

	Window window = Window(props);
	window.create();

	ResourceManager::LoadShader("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl", "basic");
	ShaderHandle test_Shader_handle = ResourceManager::GetShader("basic");

	TextureParameters tParams;
	int channels;
	tParams.imageData = stbi_load("assets/textures/dirt.png", &tParams.width, &tParams.height, &channels, 4);

	auto backend = useOpenGL ? std::make_unique<OpenGLBackend>() : nullptr;
	Renderer renderer = Renderer(std::move(backend));

	TextureHandle tHandle = renderer.CreateTexture(tParams);
	MeshHandle quad = renderer.CreateMesh(GetUnitQuad());
	MeshHandle cube = renderer.CreateMesh(GetUnitCube());

	Input input;
	/*window.setInputHandler(&input);
	window.setKeyCallback(Input::keyCallback);
	window.setResizeCallback(Input::resizeCallback);*/
	//TODO: Make window methods
	glfwSetMouseButtonCallback(window.getWindow(), Input::mouseButtonCallback);
	glfwSetCursorPosCallback(window.getWindow(), Input::cursorPosCallback);

	Camera camera;
	camera.projection = ProjectionType::Perspective;
	camera.position = {0,0,5};
	camera.orthoSize = 3.0f;
	camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

	glfwSwapInterval(0);

	double lastTime = glfwGetTime();

	const siv::PerlinNoise::seed_type seed = 12345;
	const siv::PerlinNoise perlin{seed};

	ChunkManager chunkManager;

	while (!window.shouldClose()) {
		double now = glfwGetTime();
		auto deltaTime = static_cast<float>(now - lastTime);
		lastTime = now;
		std::string title = "Block Game ";
		title.append(std::to_string(1.0f/deltaTime));
		glfwSetWindowTitle(window.getWindow(), title.c_str());

		glfwPollEvents();

		camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

		const float lookSensitivity = 0.15f;
		float camSpeed = 6.0f;

		if (input.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) camSpeed *= 2;

		if (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			glm::vec2 mouseDelta = input.ConsumeMouseDelta();
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

			if (input.IsKeyDown(GLFW_KEY_W)) camera.position += forward * camSpeed * deltaTime;
			if (input.IsKeyDown(GLFW_KEY_S)) camera.position -= forward * camSpeed * deltaTime;
			if (input.IsKeyDown(GLFW_KEY_A)) camera.position -= right * camSpeed * deltaTime;
			if (input.IsKeyDown(GLFW_KEY_D)) camera.position += right * camSpeed * deltaTime;
		} else {
			glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			input.ConsumeMouseDelta();
		}

		if (input.IsKeyDown(GLFW_KEY_F11)) {
			glfwSetWindowSize(window.getWindow(), 1920, 1080);
		}

		chunkManager.Update(camera.position, renderer, perlin);

		renderer.Begin(camera);

		chunkManager.Render(renderer, test_Shader_handle, tHandle);

		renderer.End();

		glfwSwapBuffers(window.getWindow());
	}

	glfwTerminate();

	return 0;
}
