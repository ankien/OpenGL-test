#pragma once

#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <string>

struct Shader {
    static const unsigned int NUM_SHADERS = 2;
    unsigned int program;
    unsigned int shaders[NUM_SHADERS];

    static std::string loadShader(const std::string& fileName) {
        std::fstream fileStream(fileName, std::ios::binary | std::ios::in);
        if(!fileStream.is_open()) {
            std::cerr << "Error loading shader\n";
            exit(1);
        }

        std::string output;
        std::string line;

        while(fileStream.good()) {
            std::getline(fileStream, line);
            output.append(line + "\n");
        }
        return output;
    }

    static unsigned int createShader(const std::string& text, GLenum shaderType) {
        unsigned int shader = glCreateShader(shaderType);
        
        const char* shaderSourceStrings[1] = {text.c_str()};
        const int shaderSourceInputLengths[1] = {text.length()};
        glShaderSource(shader, 1, shaderSourceStrings, shaderSourceInputLengths);
        glCompileShader(shader);

        return shader;
    }

    void bind() {
        glUseProgram(program);
    }

    Shader(const std::string& fileName) {
        program = glCreateProgram();
        shaders[0] = createShader(loadShader(fileName + ".vrts"), GL_VERTEX_SHADER);
        shaders[1] = createShader(loadShader(fileName + ".frgs"), GL_FRAGMENT_SHADER);
        
        for(unsigned int i = 0; i < NUM_SHADERS; i++)
            glAttachShader(program, shaders[i]);

        glBindAttribLocation(program, 0, "position");
        glLinkProgram(program);
    }

    ~Shader() {
        for(unsigned int i = 0; i < NUM_SHADERS; i++) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }

        glDeleteProgram(program);
    }
};