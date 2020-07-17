#include <iostream>
#include <GL/glew.h>
#include "SDL.h"

void toggleFullscreen(SDL_Window* window) {
    uint32_t fullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
    SDL_ShowCursor(isFullscreen);
}

#undef main // because SDL has a main macro for initialization for some reason

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_Window* window = SDL_CreateWindow(
        "Mah Project", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        1280, 
        720, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    GLenum status = glewInit();
    
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    bool exit = false;
    SDL_Event event;
    while(!exit){ // draw loop
        while(SDL_PollEvent(&event)) {
            glClear(GL_COLOR_BUFFER_BIT);
            SDL_GL_SwapWindow(window);

            if(event.type == SDL_QUIT) exit = true;

            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_ESCAPE) exit = true;
                if(event.key.keysym.sym == SDLK_f) toggleFullscreen(window);
            }
        }
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}