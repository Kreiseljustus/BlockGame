#include "Window.h"

#include <iostream>

void Window::create() {

    glfwWindowHint(GLFW_VISIBLE, !m_Hidden);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to initialize GLFW: " << std::endl;
        return;
    }

    glfwMakeContextCurrent(m_Window);
    std::cout << "Context current window: " << glfwGetCurrentContext() << std::endl;

    glewExperimental = GL_TRUE;
    if (const GLenum err = glewInit(); err != GLEW_OK) {
        glGetError();
        std::cerr << "[Ignore on Wayland] Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return;
    }
}

void Window::destroy() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}

void Window::setInputHandler(Input *input) const{
    glfwSetWindowUserPointer(m_Window, input);
}

void Window::setKeyCallback(const GLFWkeyfun callback) const {
    glfwSetKeyCallback(m_Window, callback);
}

void Window::setResizeCallback(const GLFWframebuffersizefun callback) const {
    glfwSetFramebufferSizeCallback(m_Window, callback);
}

Vector2I Window::getFrameBufferSize() const {
    Vector2I size;
    glfwGetFramebufferSize(m_Window, &size.x, &size.y);

    return size;
}
