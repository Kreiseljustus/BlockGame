#include <iostream>

#include <Window.h>

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
    }

    WindowProperties props;
    props.width = 800;
    props.height = 600;
    props.hidden = false;
    props.title = "Block Game";

    Window window = Window(props);
    window.create();

    while (!window.shouldClose()) {
        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        glfwSwapBuffers(window.getWindow());
    }

    glfwTerminate();

    return 0;
}