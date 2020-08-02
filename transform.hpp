#pragma once

#include <glm.hpp>
#include <gtx/transform.hpp>

struct Transform {
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;

     glm::mat4 getModel() const {
        glm::mat4 posMatrix = glm::translate(pos);

        glm::mat4 rotXMatrix = glm::rotate(rot.x,glm::vec3(1.0,0.0,0.0));
        glm::mat4 rotYMatrix = glm::rotate(rot.y,glm::vec3(0.0,1.0,0.0));
        glm::mat4 rotZMatrix = glm::rotate(rot.z,glm::vec3(0.0,0.0,1.0));
        glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

        glm::mat4 scaleMatrix = glm::scale(scale);

        return posMatrix * rotMatrix * scaleMatrix;
    }

     Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0)) {
        this->pos=pos;
        this->rot=rot;
        this->scale=scale;
     }
};