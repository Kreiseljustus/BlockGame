#ifndef BLOCKGAME_WINDOW_H
#define BLOCKGAME_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Input.h"
#include "math/Vector2.h"

namespace Engine {
    struct WindowProperties {
        int Width = 600, Height = 600;
        std::string Title = "New Window";
        bool Hidden = false;
        bool CreateOpenGLContext = true;
    };

    class Window {
    public:
        explicit Window(const WindowProperties& properties) : m_Title(properties.Title), m_Width(properties.Width)
        , m_Height(properties.Height), m_Hidden(properties.Hidden), m_CreateOpenGLContext(properties.CreateOpenGLContext) {}

        ~Window() {destroy();}

        void create();
        void destroy();

        void setInputHandler(Input* input) const;
        void setKeyCallback(GLFWkeyfun callback) const;
        void setResizeCallback(GLFWframebuffersizefun callback) const;

        [[nodiscard]] Vector2I getFrameBufferSize() const;

        [[nodiscard]] bool isHidden() const {return m_Hidden;};
        void setHidden(const bool hidden) {m_Hidden = hidden;}

        [[nodiscard]] bool shouldClose() const {return glfwWindowShouldClose(m_Window);}

        [[nodiscard]] GLFWwindow* getWindow() const {return m_Window;}
    private:
        GLFWwindow* m_Window{nullptr};
        std::string m_Title;
        int m_Width, m_Height;
        bool m_Hidden;
        bool m_CreateOpenGLContext;
    };
}


#endif //BLOCKGAME_WINDOW_H
