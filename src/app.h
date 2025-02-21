#pragma once
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <array>


class App {
   public:
    App(int w, int h);
    /* Initializes de application libraries and constants */
    bool init();
    /* Runs the core logic */
    bool run();
    /* Safely closes the application */
    bool close();

   private:
    int max_fps = 120;
    int SCREEN_WIDTH, SCREEN_HEIGHT;
    std::array<unsigned int, 2> gShaders;
    std::array<unsigned int, 2> gVAOs;
    std::array<unsigned int, 2> gVBOs;
    bool quit = false;
    SDL_Window *gWindow = NULL;
    SDL_Surface *gScreenSurface = NULL;
    SDL_GLContext gContext = NULL;
    /* Draws to the screen */
    bool draw();
    /* Handles SDL Events */
    bool handleEvent(SDL_Event e);
    /* Handles an SDL Event */
    void handleWindowEvent(SDL_WindowEvent e);
    bool initOpenGL();
};