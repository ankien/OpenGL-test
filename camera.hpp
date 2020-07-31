#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtx/transform.hpp>

struct Camera {
    glm::mat4 projection;
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 forward;
    
    void moveForward(float amount) {
        pos += forward * amount;
    }

    void moveRight(float amount) {
        
    }

    void pitchUp(float angle) {
        
    }

    void yawRight(float angle) {
        
    }

    glm::mat4 getViewProjection() const {
        return projection * glm::lookAt(pos,pos+forward,up); // proj * view
    }
    
    Camera(float fovDeg, uint16_t width, uint16_t height, float near, float far, bool orthoMode) {
        if(orthoMode) {
            this->projection = glm::ortho(0.0f,(float)width,0.0f,(float)height,near,far);
        } else {
            this->projection = glm::perspective(glm::radians(fovDeg),(float)width/(float)height,near,far);
        }

        this->pos = glm::vec3(0.0f,0.0f,-1.5f);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
    }
};