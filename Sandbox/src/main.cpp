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
	std::cout << ResourceManager::GetShader("basic").handle;

	auto backend = useOpenGL ? std::make_unique<OpenGLBackend>() : nullptr;
	Renderer renderer = Renderer(std::move(backend));

	Input input;
	window.setInputHandler(&input);
	window.setKeyCallback(Input::keyCallback);
	window.setResizeCallback(Input::resizeCallback);

	while (!window.shouldClose()) {
		glfwPollEvents();
		renderer.Begin();

		//renderer.Submit({0}, {0,0}, {{},{}});

		renderer.End();

		glfwSwapBuffers(window.getWindow());
	}

	glfwTerminate();

	return 0;
}
