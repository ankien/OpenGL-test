#pragma once

#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <string>
#include "transform.hpp"
#include "camera.hpp"

struct Shader {
    enum {
        TRANSFORM_U,
        LIGHTCOLOR_U,
        // can add more uniforms from shaders
        NUM_UNIFORMS
    };

    static const unsigned int NUM_SHADERS = 2; // how many type of shaders for each object/surface? (geometry,vertex,etc.)
    unsigned int program;
    unsigned int shaders[NUM_SHADERS];
    unsigned int uniforms[NUM_UNIFORMS]; // # not including textures since they don't need to be updated

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

    void use() {
        glUseProgram(program);
    }

    void update(const Transform& transform, const Camera& camera, glm::vec3 lightColor) {
        glm::mat4 model = camera.getViewProjection() * transform.getModel(); // MVP
        glUniformMatrix4fv(uniforms[TRANSFORM_U],1,false,&model[0][0]);
        glUniform3fv(uniforms[LIGHTCOLOR_U],1,&lightColor[0]);
    }

    Shader(const std::string& fileName) {
        this->program = glCreateProgram();
        this->shaders[0] = createShader(loadShader(fileName + ".vrts"), GL_VERTEX_SHADER);
        this->shaders[1] = createShader(loadShader(fileName + ".frgs"), GL_FRAGMENT_SHADER);
        
        for(unsigned int i = 0; i < NUM_SHADERS; i++)
            glAttachShader(program, shaders[i]);

        glLinkProgram(program);

        this->uniforms[TRANSFORM_U] = glGetUniformLocation(program,"transform");
        this->uniforms[LIGHTCOLOR_U] = glGetUniformLocation(program, "lightColor");
    }

    ~Shader() {
        for(unsigned int i = 0; i < NUM_SHADERS; i++) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }

        glDeleteProgram(program);
    }
};
