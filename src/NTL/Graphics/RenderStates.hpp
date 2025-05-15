#ifndef NTL__RENDER_STATES_HPP
#define NTL__RENDER_STATES_HPP

#include <glad/glad.h>
#include <string_view>
#include "../Utils/Matrix.hpp"
#include "Shader.hpp"

namespace ntl {

class RenderStates {
public:
    RenderStates() = default;

    RenderStates(const RenderStates &other) = default;

    RenderStates(const Matrix4x4f &transform) :
        transform(&transform)
    {}

    RenderStates(const Shader &shader) :
        shader(&shader)
    {}

    RenderStates(const Matrix4x4f &transform, const Shader &shader) :
        transform(&transform),
        shader(&shader)
    {}

public:
    const Shader *shader{};
    const Matrix4x4f *transform = &Matrix4x4f::Identity;

    static const RenderStates Default;
};

}

#endif // NTL__RENDER_STATES_HPP