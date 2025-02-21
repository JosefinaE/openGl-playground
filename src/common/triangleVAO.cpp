#include <app.h>

#include <array>
#include <common/loadShaders.cpp>
#include <iostream>

std::array<unsigned int, 2> loadTriangleVAO(std::array<float, 9> vertices) {
    unsigned int VBO;  // Vertex Buffer Objects, stores vertices on GPU memory
    unsigned int VAO;  // Vertex Array Objects, stores state configuration
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // now buffer calls (on the GL_ARRAY_BUFFER target) will act on VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);  // copy to buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // it operates on the bound VBO
    glEnableVertexAttribArray(0); // unbind VAO
    return {VAO, VBO};
}

unsigned int loadRectEBO() {
    float vertices[] = {
        0.5f, 0.5f, 0.0f,    // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f, 0.0f    // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;  // Element Buffer Objects, stores indices for drawing vertices

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //
    glEnableVertexAttribArray(0);
    return VAO;
}

/* returns shader program id*/
unsigned int loadShaderProgram(const char* vertPath, const char* fragPath) {
    unsigned int vertexShader = loadShader(vertPath, GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        return 0;
    }
    unsigned int fragmentShader = loadShader(fragPath, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        return 0;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);  // we attach shaders to shader program and link them
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM_FAILED\n"
                  << infoLog << std::endl;
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}