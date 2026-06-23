//
// Created by crisel on 6/23/26.
//

#include "Shader.h"

#include <fstream>
#include <iostream>

#include "GL/glew.h"

void Shader::bind() {
    glUseProgram(m_ShaderProgramID);
}

bool Shader::load(std::filesystem::path vertexShader, std::filesystem::path fragmentShader) {
    ShaderType vertexType;
    std::string vertexSrc;

    loadFromFile(vertexShader, &vertexType, &vertexSrc);

    if (vertexType != ShaderType::VERTEX_SHADER) {
        std::cout << vertexShader << " is not of type Vertex Shader! (Shader::load)" << std::endl;
        return false;
    }

    ShaderType fragmentType;
    std::string fragmentSrc;

    loadFromFile(fragmentShader, &fragmentType, &fragmentSrc);

    if (fragmentType != ShaderType::FRAGMENT_SHADER) {
        std::cout << fragmentShader << " is not of type Fragment Shader! (Shader::load)" << std::endl;
    }

    unsigned int vertexShaderID = compileShader(vertexSrc, ShaderType::VERTEX_SHADER);
    unsigned int fragmentShaderID = compileShader(fragmentSrc, ShaderType::FRAGMENT_SHADER);

    if (vertexShaderID == 0 || fragmentShaderID == 0) {
        std::cout << "Something went wrong whilst compiling shaders! (Shader::load)" << std::endl;
        return false;
    }

    m_ShaderProgramID = linkShaderProgram(vertexShaderID, fragmentShaderID);

    return true;
}

void Shader::loadFromFile(std::filesystem::path path, ShaderType *typeOut, std::string *sourceOut) {
    std::ifstream file(path, std::ios::in);

    std::stringstream stream;
    stream << file.rdbuf();

    bool foundType = false;
    std::string line;
    while (getline(stream, line) && !foundType) {
        if (line == "#vertex") {*typeOut = ShaderType::VERTEX_SHADER; foundType = true;}
        if (line == "#fragment") {*typeOut = ShaderType::FRAGMENT_SHADER; foundType = true;}
    }

    if (!foundType) {std::cout << "Failed to find shader type for " << path << std::endl; return;}

    *sourceOut = stream.str();
}

unsigned int Shader::compileShader(std::string src, ShaderType type) {
    unsigned int id = glCreateShader(type == ShaderType::VERTEX_SHADER ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "Shader compilation failed! " << std::endl << infoLog << std::endl;
    }

    return id;
}

unsigned int Shader::linkShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    const unsigned int id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "Shader linking failed! " << std::endl; infoLog << std::endl;
    }

    return id;
}
