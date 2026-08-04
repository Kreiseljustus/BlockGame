#include <iostream>

#include <Window.h>
#include <Input.h>

#include <ResourceManager.h>
#include <Rendering/Renderer.h>
#include <Rendering/Backends/OpenGLBackend.h>

using namespace Engine::Rendering;
using namespace Engine;

int main(int arc, char* argv[]) {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
	}

	volatile bool useOpenGL = true;

	WindowProperties props;
	props.width = 800;
	props.height = 600;
	props.hidden = false;
	props.title = "Block Game";

	Window window = Window(props);
	window.create();

	std::cout << ResourceManager::LoadShader("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl", "basic").handle;

	std::cout << std::endl;
	ShaderHandle test_Shader_handle = ResourceManager::GetShader("basic");

	MeshData test_data = ResourceManager::LoadMesh("assets/meshes/test.fbx", "monkey");


	auto backend = useOpenGL ? std::make_unique<OpenGLBackend>() : nullptr;
	Renderer renderer = Renderer(std::move(backend));

	MeshHandle test_handle = renderer.CreateMesh(test_data);

	Input input;
	window.setInputHandler(&input);
	window.setKeyCallback(Input::keyCallback);
	window.setResizeCallback(Input::resizeCallback);

	int worldHeight = 500;

	Camera camera;
	camera.projection = ProjectionType::Orthographic;
	camera.position = {0,0,5};
	camera.orthoSize = 3.0f;
	camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

	double lastTime = glfwGetTime();

	while (!window.shouldClose()) {
		double now = glfwGetTime();
		float deltaTime = static_cast<float>(now - lastTime);
		lastTime = now;

		glfwPollEvents();
		camera.aspect = static_cast<float>(window.getFrameBufferSize().x) / static_cast<float>(window.getFrameBufferSize().y);

		const float speed = 2.0f;
		if (input.IsKeyDown(GLFW_KEY_W)) camera.position.y += speed * deltaTime;
		if (input.IsKeyDown(GLFW_KEY_S)) camera.position.y -= speed * deltaTime;
		if (input.IsKeyDown(GLFW_KEY_A)) camera.position.x -= speed * deltaTime;
		if (input.IsKeyDown(GLFW_KEY_D)) camera.position.x += speed * deltaTime;
		if (input.IsKeyDown(GLFW_KEY_LEFT)) camera.rotation.y += 10 * deltaTime;
		if (input.IsKeyDown(GLFW_KEY_RIGHT)) camera.rotation.y -= 10 *deltaTime;

		renderer.Begin(camera);

		for (int i = 0; i < 100; i++) {
			glm::vec3 pos = {i * 3,0,0.7};
			renderer.Submit(test_handle, {test_Shader_handle.handle, 0}, {pos, {270,0,0}, {1,1,1}});
		}

		renderer.End();

		glfwSwapBuffers(window.getWindow());
	}

	glfwTerminate();

	return 0;
}
