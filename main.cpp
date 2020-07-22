#include "SDL.h"
#include "shader.hpp"
#include "mesh.hpp"

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
        "OpenGL Test", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        1280, 
        720, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    GLenum status = glewInit();
    
    /// stuff to draw
    Shader shader("./shaders/myShader");

    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    Vertex vertices[] = { Vertex(glm::vec3(-0.5,-0.5,0)),
                           Vertex(glm::vec3(0,0.5,0)),
                           Vertex(glm::vec3(0.5,-0.5,0)) };
    Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));
    ///
    
    /// event/control variables
    bool exit = false;
    const uint8_t* state;
    SDL_Event event;
    ///

    while(!exit){ // draw loop
        glUseProgram(shader.program);
        while(SDL_PollEvent(&event)) { // update display
            glClear(GL_COLOR_BUFFER_BIT);

            shader.bind();
            mesh.draw();

            SDL_GL_SwapWindow(window);

            if(event.type == SDL_QUIT) exit = true;

            state = SDL_GetKeyboardState(NULL);
            if(event.type == SDL_KEYDOWN) {
                if(state[SDL_SCANCODE_ESCAPE]) exit = true;
                if(state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_RETURN]) toggleFullscreen(window);
            }
        }
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}