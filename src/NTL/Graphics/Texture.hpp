#ifndef NTL__TEXTURE_HPP
#define NTL__TEXTURE_HPP

#include <glad/glad.h>
#include <string_view>

#include "../Utils/Rectangle.hpp"
#include "../Utils/Color.hpp"

namespace ntl {

struct Shader;

class Texture {
public:
    Texture() = default;

    Texture(std::string_view fileName) {
        loadFromFile(fileName);
    }


    ~Texture();


    void loadFromFile(std::string_view fileName);


    Vector2i getSize() const {
        return size_;
    }

protected:
    friend Shader;

    Vector2i size_;
    GLuint texture_;
};

}

#endif // NTL__TEXTURE_HPP