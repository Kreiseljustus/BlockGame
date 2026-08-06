//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_SHADER_H
#define BLOCKGAME_SHADER_H
#include <filesystem>

namespace Engine::Rendering {
    enum ShaderType : char {
        VERTEX_SHADER = 1,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER
    };

    class Shader {
    public:
        Shader() = default;
        ~Shader();

        void bind() const;

        bool load(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader);

        [[nodiscard]] unsigned int id() const {return m_ShaderProgramID;}

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& other) noexcept : m_ShaderProgramID(other.m_ShaderProgramID) {
            other.m_ShaderProgramID = 0;
        }
        Shader& operator=(Shader&& other) noexcept {
            if (this != &other) {
                m_ShaderProgramID = other.m_ShaderProgramID;
                other.m_ShaderProgramID = 0;
            }
            return *this;
        }
    private:
        static void loadFromFile(const std::filesystem::path& path, ShaderType* typeOut, std::string* sourceOut);

        static unsigned int compileShader(const std::string& src, ShaderType type);
        static unsigned int linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
    private:
        unsigned int m_ShaderProgramID = 0;
    };
}

#endif //BLOCKGAME_SHADER_H
