//
// Created by crisel on 6/23/26.
//

#ifndef BLOCKGAME_SHADER_H
#define BLOCKGAME_SHADER_H
#include <filesystem>

enum ShaderType : char {
    VERTEX_SHADER = 1,
    FRAGMENT_SHADER,
    GEOMETRY_SHADER
};

class Shader {
public:
    void bind();

    bool load(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader);

    [[nodiscard]] unsigned int id() {return m_ShaderProgramID;}
private:
    void loadFromFile(const std::filesystem::path& path, ShaderType* typeOut, std::string* sourceOut);
    unsigned int compileShader(const std::string& src, ShaderType type);
    unsigned int linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
private:
    unsigned int m_ShaderProgramID = 0;
};


#endif //BLOCKGAME_SHADER_H
