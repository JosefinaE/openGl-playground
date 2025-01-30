#pragma once
#include <glad/glad.h>
#include <SDL2/SDL.h>

class App
{
public:
    SDL_Window *gWindow = NULL;
    SDL_Surface *gScreenSurface = NULL;
    SDL_GLContext gContext = NULL;
    App(int w, int h);
    bool init();
    bool run();
    bool close();

private:
    int SCREEN_WIDTH, SCREEN_HEIGHT;
};