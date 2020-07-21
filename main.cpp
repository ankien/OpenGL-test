#include "SDL.h"
#include "Shader.hpp"

/*
struct Point2D{
    int x, y;
};

// Finds the determinant of:
// |ax bx cx|   |bx-ax cx-ax|
// |ay by cy| = |by-ay cy-ay|
// | 1  1  1|
int orientation2D(const Point2D& a, const Point2D& b, const Point2D& c) {
    return ((b.x-a.x)*(c.y-a.y)) - ((b.y-a.y)*(c.x-a.x));
}


void drawTriangle(const Point2D& v0, const Point2D& v1, const Point2D& v2) {
    // Compute triangle bounding box
    int minX = min(v0.x, v1.x, v2.x);
    int minY = min(v0.y, v1.y, v2.y);
    int maxX = max(v0.x, v1.x, v2.x);
    int maxY = max(v0.y, v1.y, v2.y);

    // Clip against screen bounds
    minX = max(minX, 0);
    minY = max(minY, 0);
    maxX = min(maxX, screenWidth - 1);
    maxY = min(maxY, screenHeight - 1);

    // Rasterize
    Point2D p;
    for (p.y = minY; p.y <= maxY; p.y++) {
        for (p.x = minX; p.x <= maxX; p.x++) {
            // Determine barycentric coordinates
            int w0 = orient2d(v1, v2, p);
            int w1 = orient2d(v2, v0, p);
            int w2 = orient2d(v0, v1, p);

            // If p is on or inside all edges, render pixel.
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
                renderPixel(p, w0, w1, w2);           
        }
    }
}
*/

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
    
    Shader shader("./shaders/myShader");
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    bool exit = false;
    SDL_Event event;
    while(!exit){ // draw loop
        glUseProgram(shader.program);
        while(SDL_PollEvent(&event)) { // update display
            glClear(GL_COLOR_BUFFER_BIT);
            SDL_GL_SwapWindow(window);

            if(event.type == SDL_QUIT) exit = true;

            const uint8_t* state = SDL_GetKeyboardState(NULL);
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