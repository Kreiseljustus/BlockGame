#ifndef BLOCKGAME_WINDOW_H
#define BLOCKGAME_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Input.h"
#include "math/Vector2.h"

namespace Engine {
    struct WindowProperties {
        int width = 600, height = 600;
        std::string title = "New Window";
        bool hidden = false;
    };

    class Window {
    public:
        explicit Window(const WindowProperties& properties) : m_Title(properties.title), m_Width(properties.width)
        , m_Height(properties.height), m_Hidden(properties.hidden) {}

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
    };
}


#endif //BLOCKGAME_WINDOW_H
