#include <app.h>
#include <iostream>

App::App(int w, int h)
{
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}

bool App::run()
{
    // Fill the surface white
    SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0x0F, 0x00, 0x00));
    // Update the surface
    SDL_UpdateWindowSurface(gWindow);
    // Hack to get window to stay up
    gContext = SDL_GL_CreateContext(gWindow);
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Event e;
    bool quit = false;
    while (quit == false)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }
    return true;
}

bool App::init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error" << SDL_GetError() << std::endl;
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        std::cout << "Window could not be created! SDL_Error" << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    // Get window surface
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return success;
}

bool App::close()
{
    SDL_FreeSurface(gScreenSurface);
    SDL_DestroyWindow(gWindow);
    gScreenSurface = NULL;
    gWindow = NULL;
    SDL_Quit();
    return true;
}