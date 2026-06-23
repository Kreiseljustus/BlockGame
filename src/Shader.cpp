//
// Created by crisel on 6/23/26.
// TODO: Add fallback shader
//

#include "Shader.h"

#include <fstream>
#include <iostream>

#include "GL/glew.h"

void Shader::bind() {
    if (m_ShaderProgramID != 0) {
        glUseProgram(m_ShaderProgramID);
    } else {
        std::cout << "Tried to bind unloaded shader!!! (Shader::bind)" << std::endl;
    }
}

bool Shader::load(const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader) {
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
        return false;
    }

    unsigned int vertexShaderID = compileShader(vertexSrc, ShaderType::VERTEX_SHADER);
    unsigned int fragmentShaderID = compileShader(fragmentSrc, ShaderType::FRAGMENT_SHADER);

    if (vertexShaderID == 0 || fragmentShaderID == 0) {
        std::cout << "Something went wrong whilst compiling shaders! (Shader::load)" << std::endl;
        return false;
    }

    m_ShaderProgramID = linkShaderProgram(vertexShaderID, fragmentShaderID);

    std::cout << "Loaded shader program from " << vertexShader << " and " << fragmentShader << std::endl;

    return true;
}

void Shader::loadFromFile(const std::filesystem::path& path, ShaderType *typeOut, std::string *sourceOut) {
    std::ifstream file(path, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << path << std::endl << "Absolute path of file: " << std::filesystem::absolute(path) << std::endl;
        return;
    }

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
    *sourceOut = sourceOut->substr(sourceOut->find('\n', 1));
}

unsigned int Shader::compileShader(const std::string& src, const ShaderType type) {
    const unsigned int id = glCreateShader(type == ShaderType::VERTEX_SHADER ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

    const GLchar* srcC = src.c_str();
    glShaderSource(id, 1, &srcC, nullptr);
    glCompileShader(id);

    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "Shader compilation failed! " << std::endl << infoLog << std::endl;
        return 0;
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
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Shader linking failed! " << std::endl << infoLog << std::endl;
        return 0;
    }

    return id;
}
