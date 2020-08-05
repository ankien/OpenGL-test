#pragma once

#include <glm.hpp>
#include <GL/glew.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texture;
    glm::vec3 normal;
    Vertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec2& texture, const glm::vec3& normal = glm::vec3(0,0,0)) :
        pos(pos),
        color(color),
        texture(texture),
        normal(normal)
    {}
};

struct Mesh {
    static const uint8_t NUM_BUFFERS = 4;
    unsigned int vertexArray; // VAOs
    unsigned int lightVertexArray;
    unsigned int vertexArrayBuffers[NUM_BUFFERS]; // VBO, since data is stored in this object we don't have to create another VBO for reused vertex shaders
    unsigned int drawCount;

    void draw(const unsigned int& AO) {
        glBindVertexArray(AO);
        glDrawArrays(GL_TRIANGLES,0,drawCount);
    }

    Mesh(Vertex* vertices, unsigned int numOfVertices) {
        this->drawCount = numOfVertices;

        glGenVertexArrays(1,&vertexArray);
        glBindVertexArray(vertexArray);
        glGenBuffers(NUM_BUFFERS,vertexArrayBuffers);
        glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffers[0]);
        glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0,3,GL_FLOAT,false,11*sizeof(float),0);

        glEnableVertexAttribArray(1); // color
        glVertexAttribPointer(1,3,GL_FLOAT,false,11*sizeof(float),(void*)(3*sizeof(float)));

        glEnableVertexAttribArray(2); // texture
        glVertexAttribPointer(2,2,GL_FLOAT,false,11*sizeof(float),(void*)(6*sizeof(float)));

        glEnableVertexAttribArray(3); // normal
        glVertexAttribPointer(3,3,GL_FLOAT,false,11*sizeof(float),(void*)(8*sizeof(float)));

        glGenVertexArrays(1,&lightVertexArray);
        glBindVertexArray(lightVertexArray);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0,3,GL_FLOAT,false,11*sizeof(float),0);

        glEnableVertexAttribArray(1); // color
        glVertexAttribPointer(1,3,GL_FLOAT,false,11*sizeof(float),(void*)(3*sizeof(float)));
    }

    ~Mesh() {
        glDeleteVertexArrays(1,&vertexArray);
    }
};