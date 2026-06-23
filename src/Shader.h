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

    bool load(std::filesystem::path vertexShader, std::filesystem::path fragmentShader);
private:
    void loadFromFile(std::filesystem::path path, ShaderType* typeOut, std::string* sourceOut);
    unsigned int compileShader(std::string src, ShaderType type);
    unsigned int linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
private:
    unsigned int m_ShaderProgramID;
};


#endif //BLOCKGAME_SHADER_H
