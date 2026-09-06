#include "Window.h"

#include <iostream>

#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

using namespace Engine;

void Window::create() {

    glfwWindowHint(GLFW_VISIBLE, !m_Hidden);

    if (m_CreateOpenGLContext) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to initialize GLFW: " << std::endl;
        return;
    }
    std::cout << "GLFW Init" << std::endl;

    glfwMakeContextCurrent(m_Window);
    std::cout << "Context current window: " << glfwGetCurrentContext() << std::endl;
    glfwSetWindowUserPointer(m_Window, this);

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* w, const int width, const int height) {
        const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        Events::WindowResizeEvent event(width,height);
        self->m_EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* w, const int key, int scancode, const int action, int mods) {
        const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (action == GLFW_PRESS) {
            Events::KeyPressedEvent event(key);
            self->m_EventCallback(event);
        } else if (action == GLFW_RELEASE) {
            Events::KeyReleasedEvent event(key);
            self->m_EventCallback(event);
        }
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* w) {
        const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        Events::WindowCloseEvent event{};
        self->m_EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* w, const int button, const int action, int mods) {
        const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        if (action == GLFW_PRESS) {
            Events::MouseButtonPressedEvent event(button);
            self->m_EventCallback(event);
        } else if (action == GLFW_RELEASE) {
            Events::MouseButtonReleasedEvent event(button);
            self->m_EventCallback(event);
        }
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* w, const double x, const double y) {
        const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        Events::MouseMovedEvent event(x,y);
        self->m_EventCallback(event);
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* w, const double xOffset, const double yOffset) {
        const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(w));
        Events::MouseScrolledEvent event(xOffset, yOffset);
        self->m_EventCallback(event);
    });

    if (m_CreateOpenGLContext) {
        glewExperimental = GL_TRUE;
        if (const GLenum err = glewInit(); err != GLEW_OK) {
            glGetError();
            std::cerr << "[Ignore on Wayland] Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
            return;
        }
        std::cout << "Glew Init" << std::endl;
    }
}

void Window::destroy() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
}
#if 0
void Window::setInputHandler(Input* input) const{
    glfwSetWindowUserPointer(m_Window, input);
}

void Window::setKeyCallback(const GLFWkeyfun callback) const {
    glfwSetKeyCallback(m_Window, callback);
}

void Window::setResizeCallback(const GLFWframebuffersizefun callback) const {
    glfwSetFramebufferSizeCallback(m_Window, callback);
}
#endif

Vector2I Window::getFrameBufferSize() const {
    Vector2I size;
    glfwGetFramebufferSize(m_Window, &size.x, &size.y);

    return size;
}
