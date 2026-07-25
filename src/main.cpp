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

    Shader shader;
    shader.load("assets/shaders/BasicVertexShader.glsl", "assets/shaders/BasicFragmentShader.glsl");
    shader.bind();

    Input input;
    window.setInputHandler(&input);
    window.setKeyCallback(Input::keyCallback);
    window.setResizeCallback(Input::resizeCallback);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/textures/test.jpg", &width, &height, &nrChannels, 0);

    float vertices[] = {
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
        0, 1 ,3,
        1,2,3
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else std::cout << "failed to load texture!" <<std::endl;

    stbi_image_free(data);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE ,8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int transformLocation = glGetUniformLocation(shader.id(), "transform");

    while (!window.shouldClose()) {
        glClearColor(0.0, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        auto transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, static_cast<float>(glfwGetTime()), glm::vec3(0.0,0.0,1.0));
        transform = glm::scale(transform, glm::vec3(2.0,2.0,2.0));

        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwPollEvents();
        glfwSwapBuffers(window.getWindow());
    }

    glfwTerminate();

    return 0;
}
