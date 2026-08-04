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

	while (!window.shouldClose()) {
		glfwPollEvents();
		renderer.Begin();

		glm::vec3 pos = {0,0,0.7};
		renderer.Submit(test_handle, {test_Shader_handle.handle, 0}, {pos, {270,0,0}, {1,1,1}});

		renderer.End();

		glfwSwapBuffers(window.getWindow());
	}

	glfwTerminate();

	return 0;
}
