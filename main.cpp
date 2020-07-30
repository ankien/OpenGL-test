#include "SDL.h"
#include "shader.hpp"
#include "mesh.hpp"
#include "texture.hpp"

void toggleFullscreen(SDL_Window* window) {
    uint32_t fullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool isFullscreen = SDL_GetWindowFlags(window) & fullscreenFlag;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : fullscreenFlag);
    SDL_ShowCursor(isFullscreen);
}

#undef main // because SDL has a main macro for initialization for some reason

int main() {
    static const int WIDTH  = 1280;
    static const int HEIGHT =  720;

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
        WIDTH, 
        HEIGHT, 
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    GLenum status = glewInit();
    
    /* stuff to draw */
    Shader shader("./shaders/myShader");     // load shaders

    Texture texture("./textures/rice.jpg");  // load textures
    Texture texture2("./textures/star.png");

    shader.bind(); // assign location values for multiple textures in fragment shader
    glUniform1i(glGetUniformLocation(shader.program,"ourTexture"),0);
    glUniform1i(glGetUniformLocation(shader.program,"ourTexture2"),1);

    glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // set bg color

    // triangle vertices  // Position                    // Color                // Texture mapping; Top - (0,0), Bottom right - (1,1)
    Vertex vertices[] = { Vertex(glm::vec3(-0.5,-0.5,0.0), glm::vec3(1.0,0.0,0.0), glm::vec2( 0.0,1.0)),
                          Vertex(glm::vec3( 0.0, 0.5,0.0), glm::vec3(0.0,1.0,0.0), glm::vec2( 0.5,0.0)),
                          Vertex(glm::vec3( 0.5,-0.5,0.0), glm::vec3(0.0,0.0,1.0), glm::vec2( 1.0,1.0)) };

    // establish mesh
    Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

    Transform transform; // call default transform constructor
    double counter = 0.0;

    bool fillMode = true; // polygon rendering mode
    /******************/
    
    /* event/control variables */
    bool exit = false;
    const uint8_t* state;
    SDL_Event event;
    /***************************/

    uint32_t debug = 0;

    while(!exit){ // draw before event handling because SDL pauses everything after
        glUseProgram(shader.program);

        glClear(GL_COLOR_BUFFER_BIT);

        transform.pos.x = sinf(counter);
        transform.pos.y = sinf(counter);
        counter += 0.01;

        shader.bind();

        shader.update(transform);

        texture.bind(0);
        texture2.bind(1);

        mesh.draw();

        SDL_GL_SwapWindow(window);

        while(SDL_PollEvent(&event)) { // handle events
            if(event.type == SDL_QUIT) exit = true;

            state = SDL_GetKeyboardState(NULL);
            if(event.type == SDL_KEYDOWN) {
                if(state[SDL_SCANCODE_ESCAPE]) exit = true;
                if((state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT]) && state[SDL_SCANCODE_RETURN]) toggleFullscreen(window);
                if(state[SDL_SCANCODE_F] && fillMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), fillMode = false;
                else if(state[SDL_SCANCODE_F] && !fillMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL), fillMode = true;
            }
        }
    }

    SDL_GL_DeleteContext(glContext);
    SDL_Quit();
    return 0;
}