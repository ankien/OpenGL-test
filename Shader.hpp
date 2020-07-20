#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <string>

struct Shader {
    static const unsigned int NUM_SHADERS = 2;

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

    Shader(std::string& fileName) {
        program = glCreateProgram();
    }

    ~Shader() {
        glDeleteProgram(program);
    }
    
    void bind() {
        
    }

    unsigned int program;
    unsigned int shaders[NUM_SHADERS];
};