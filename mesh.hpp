#pragma once

#include <glm.hpp>
#include <GL/glew.h>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texture;
    Vertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec2& texture) :
        pos(pos),
        color(color),
        texture(texture)
    {}
};

struct Mesh {
    enum {
        POSITION_VB,
        NUM_BUFFERS
    };

    unsigned int vertexArray;
    unsigned int vertexArrayBuffers[NUM_BUFFERS];
    unsigned int drawCount;

    void draw() {
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES,0,drawCount);
        glBindVertexArray(0);
    }

    Mesh(Vertex* vertices, unsigned int numOfVertices) {
        this->drawCount = numOfVertices;

        glGenVertexArrays(1,&vertexArray);
        glBindVertexArray(vertexArray);

        glGenBuffers(NUM_BUFFERS,vertexArrayBuffers);
        glBindBuffer(GL_ARRAY_BUFFER,vertexArrayBuffers[POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // first attribute of Vertex
        glVertexAttribPointer(0,3,GL_FLOAT,false,8*sizeof(float),0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,false,8*sizeof(float),(void*)(3*sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT,false,8*sizeof(float),(void*)(6*sizeof(float)));

        glBindVertexArray(0); // unbind
    }

    ~Mesh() {
        glDeleteVertexArrays(1,&vertexArray);
    }
};