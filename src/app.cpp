#include <app.h>

#include <common/triangleVAO.cpp>
#include <iostream>

int xMouse, yMouse = 0;
const int FPS = 60;
const int frameDelay = 1000 / FPS;

Uint32 prevTick, tick;
App::App(int w, int h) {
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}

bool App::run() {
    tick = SDL_GetTicks();
    Uint32 delta = tick - prevTick;

    SDL_Event e;  // TODO: test if event should be inside capped portion
    while (SDL_PollEvent(&e)) {
        handleEvent(e);
    }

    if (delta > frameDelay) {
        bool drawn = draw();

        if (!drawn) {
            std::cout << "Screen could not be drawn" << std::endl;
            quit = true;
        }
        prevTick = tick;
    }

    return !quit;
}

bool App::draw() {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // tell openGL size of rendering window

    int mousePosLocation = glGetUniformLocation(gShaders[1], "mousePos");
    int tickLocation = glGetUniformLocation(gShaders[1], "time");
    float xMouseNormalized = (float)xMouse / (float)SCREEN_WIDTH;
    float yMouseNormalized = (float)yMouse / (float)SCREEN_HEIGHT;
    // trig 1
    glBindVertexArray(gVAOs[0]);
    glUseProgram(gShaders[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // trig 2
    glBindVertexArray(gVAOs[1]);
    glUseProgram(gShaders[1]);
    glUniform2f(mousePosLocation, xMouseNormalized, yMouseNormalized);
    glUniform1i(tickLocation, tick);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);  // unbind vao
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0); // for polygon VAO
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
        case SDL_MOUSEMOTION:
            SDL_GetGlobalMouseState(&xMouse, &yMouse);
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
    prevTick = SDL_GetTicks();
    if (!success) {
        return false;
    }
    gShaders[0] = loadShaderProgram("../shaders/hello.vert", "../shaders/hello.frag");
    gShaders[1] = loadShaderProgram("../shaders/hello.vert", "../shaders/trig2.frag");
    std::array<unsigned int, 2> trig1 = loadTriangleVAO({
        // positions         // colors
        -0.9f, -0.9f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.9f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom left
        -0.45f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f   // top
    });

    std::array<unsigned int, 2> trig2 = loadTriangleVAO({
        // positions         // colors
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f     // top
    });
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
    gWindow = SDL_CreateWindow("Mi App :3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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
    glDeleteVertexArrays(gVAOs.size(), gVAOs.data());
    glDeleteBuffers(gVBOs.size(), gVBOs.data());
    glDeleteProgram(gShaders[0]);
    glDeleteProgram(gShaders[1]);
    SDL_DestroyWindow(gWindow);
    SDL_GL_DeleteContext(gContext);
    gContext = NULL;
    gWindow = NULL;
    SDL_Quit();
    return true;
}