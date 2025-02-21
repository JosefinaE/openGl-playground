#include <app.h>

#include <common/triangleVAO.cpp>
#include <iostream>

App::App(int w, int h) {
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}

bool App::run() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        handleEvent(e);
    }
    bool drawn = draw();
    if (!drawn) {
        std::cout << "Screen could not be drawn" << std::endl;
        quit = true;
    }

    return !quit;
}

bool App::draw() {
    glUseProgram(gShaders[0]);
    glBindVertexArray(gVAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(gShaders[1]);
    glBindVertexArray(gVAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // for polygon VAO
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // tell openGL size of rendering window
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function
    // glClear(GL_COLOR_BUFFER_BIT); // state-using function
    SDL_GL_SwapWindow(gWindow);

    return true;
}

bool App::handleEvent(SDL_Event e) {
    switch (e.type) {
        case SDL_QUIT:
            quit = true;
        case SDL_WINDOWEVENT:
            handleWindowEvent(e.window);
    }

    return true;
}

void App::handleWindowEvent(SDL_WindowEvent e) {
    switch (e.event) {
        // case SDL_WINDOWEVENT_SIZE_CHANGED:
        // case SDL_WINDOWEVENT_MAXIMIZED:
        case SDL_WINDOWEVENT_RESIZED: {
            SCREEN_WIDTH = e.data1;
            SCREEN_HEIGHT = e.data2;
        }
    }
}

bool App::init() {
    bool success = initOpenGL();
    if (!success) {
        return false;
    }
    gShaders[0] = loadShaderProgram("../shaders/hello.vert", "../shaders/hello.frag");
    gShaders[1] = loadShaderProgram("../shaders/hello.vert", "../shaders/trig2.frag");
    std::array<unsigned int, 2> trig1 = loadTriangleVAO({-0.9f, -0.9f, 0.0f,
                                                        -0.9f, 0.0f, 0.0f,
                                                        -0.45f, 0.0f, 0.0f});

    std::array<unsigned int, 2> trig2 = loadTriangleVAO({-0.5f, -0.5f, 0.0f,
                                                        0.5f, -0.5f, 0.0f,
                                                        0.0f, 0.5f, 0.0f});
    gVAOs[0] = trig1[0];
    gVBOs[0] = trig1[1];
    gVAOs[1] = trig2[0];
    gVBOs[1] = trig2[1];

    if (gVAOs[0] == 0) {
        return false;
    }
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
    return true;
}

bool App::initOpenGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error" << SDL_GetError() << std::endl;
        return false;
    }
    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!gWindow) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    //  create openGL context
    gContext = SDL_GL_CreateContext(gWindow);
    if (!gContext) {
        std::cout << "OpenGL Context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
        SDL_Quit();
        return false;
    }
    // use glad to load the OpenGL function pointers
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        close();
        return false;
    }
    return true;
}

bool App::close() {
    glDeleteVertexArrays(gVAOs.size(),gVAOs.data());
    glDeleteBuffers(gVBOs.size(),gVBOs.data());
    glDeleteProgram(gShaders[0]);
    glDeleteProgram(gShaders[1]);
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gContext);
    gContext = NULL;
    gWindow = NULL;
    SDL_Quit();
    return true;
}