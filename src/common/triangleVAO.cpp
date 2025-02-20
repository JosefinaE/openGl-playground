#include <app.h>

#include <common/loadShaders.cpp>
#include <iostream>

unsigned int loadVAO() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Vertex Buffer Objects stores vertices on GPU memory
    unsigned int VBO;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                         // now buffer calls (on the GL_ARRAY_BUFFER target) will act on VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // copy to buffer

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return VAO;
}

/* returns shader program id*/
unsigned int loadShaderProgram() {
    unsigned int vertexShader = loadShader("../shaders/hello.vert", GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        return 0;
    }
    unsigned int fragmentShader = loadShader("../shaders/hello.frag", GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        return 0;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
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