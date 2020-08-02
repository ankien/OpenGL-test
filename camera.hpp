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
        pos += glm::cross(up,forward) * amount;
    }

    void pitchDown(float angle) {
        glm::vec3 right = glm::normalize(glm::cross(up,forward));
        forward = glm::vec3(glm::normalize(glm::rotate(angle,right) * glm::vec4(forward,0)));
        up = glm::normalize(glm::cross(forward,right));
    }

    void yawLeft(float angle) {
        static const glm::vec3 TRUE_NORTH(0,1,0);
        glm::mat4 rotation = glm::rotate(angle,TRUE_NORTH);
        forward = glm::vec3(glm::normalize(rotation*glm::vec4(forward,0)));
        up = glm::vec3(glm::normalize(rotation*glm::vec4(up,0)));
    }

    glm::mat4 getViewProjection() const { // VP
        return projection * glm::lookAt(pos,pos+forward,up);
    }
    
    Camera(glm::vec3 pos, float fovDeg, uint16_t width, uint16_t height, float near, float far, bool orthoMode) {
        if(orthoMode) {
            this->projection = glm::ortho(0.0f,(float)width/(float)height,0.0f,1.0f,0.0f-near,far);
        } else {
            this->projection = glm::perspective(glm::radians(fovDeg),(float)width/(float)height,near,far);
        }

        this->pos = pos;
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
    }
};