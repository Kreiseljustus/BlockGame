#include <iostream>

#include <Window.h>
#include <stb_image.h>

#include "Input.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rendering/Renderer.h"
#include "Rendering/Backends/OpenGLBackend.h"

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

    auto backend = useOpenGL ? std::make_unique<OpenGLBackend>() : nullptr;
    Renderer renderer = Renderer(std::move(backend));

    Input input;
    window.setInputHandler(&input);
    window.setKeyCallback(Input::keyCallback);
    window.setResizeCallback(Input::resizeCallback);

    while (!window.shouldClose()) {

        renderer.Begin();

        renderer.Submit({0}, {0,0}, {{},{}});

        renderer.End();

        glfwPollEvents();
        glfwSwapBuffers(window.getWindow());
    }

    glfwTerminate();

    return 0;
}
