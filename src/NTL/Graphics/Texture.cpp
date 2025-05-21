#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>
#include <stdexcept>

ntl::Texture::~Texture() {
    glDeleteTextures(1, &texture_);
}

void ntl::Texture::loadFromFile(std::string_view fileName)
{
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int nrChannels_{};
    stbi_uc *data = stbi_load(fileName.data(), &size_.x, &size_.y, &nrChannels_, 0);

    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels_ == 1) {
            format = GL_RED;
        } else if (nrChannels_ == 3) {
            format = GL_RGB;
        } else if (nrChannels_ == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, size_.x, size_.y, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        throw std::runtime_error("Failed to load texture " + std::string(fileName));
    }
}