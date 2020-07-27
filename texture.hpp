#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <GL/glew.h>
#include "stb_image.h"

struct Texture {
    unsigned int texture;

    void bind(unsigned int unit) {
        // at most 16 textures in one pass
        assert(unit <= 15);

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    Texture(const std::string& fileName) {
        int width, height, numComponents;
        unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

        assert(imageData != NULL);

        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(imageData);
    }

    ~Texture() {
        glDeleteTextures(1,&texture);
    }
};