#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/* Loads a shader from the given path, compiles it and returns its id */
unsigned int loadShader(const char* shaderPath, const int shaderType) {
    unsigned int vertexShader;  // ID referencing the shader
    vertexShader = glCreateShader(shaderType);
    std::string shaderSource;
    std::ifstream shaderStream(shaderPath);
    if (shaderStream.is_open()) {
        std::stringstream buf;
        buf << shaderStream.rdbuf();
        shaderSource = buf.str();
        shaderStream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", shaderPath);
        getchar();
        return 0;
    }

    char const* sourcePointer = shaderSource.c_str();

    glShaderSource(vertexShader, 1, &sourcePointer, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }

    return vertexShader;
}