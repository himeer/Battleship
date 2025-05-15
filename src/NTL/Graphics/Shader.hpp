#ifndef NTL__SHADER_HPP
#define NTL__SHADER_HPP

#include <glad/glad.h>
#include <string_view>
#include "../Utils/Matrix.hpp"

namespace ntl {

class Shader {
public:
    Shader() :
        Shader(DefaultVertexSource, DefaultFragmentSource)
    {}

    Shader(std::nullptr_t) {}

    Shader(const char *vertexSrc, const char *fragmentSrc);

    ~Shader();


    Shader &operator=(Shader &&other) {
        program_ = other.program_;
        other.program_ = 0;
        return *this;
    }


    void use() const;

    void setUniform(std::string_view name, const Vector2f &value) const;

    void setUniform(std::string_view name, const Matrix4x4f &value) const;

public:
    static const char *DefaultVertexSource;
    static const char *DefaultFragmentSource;

protected:
    GLuint program_{};
};

}

#endif // NTL__SHADER_HPP