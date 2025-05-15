#include "Shader.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

const char *ntl::Shader::DefaultVertexSource = R"glsl(
#version 330 core
layout (location = 0) in vec2 pos_in;
layout (location = 1) in vec4 color_in;

uniform mat4 ntl_view;
uniform mat4 ntl_trans;

out vec4 vertColor;

void main() {
    gl_Position = ntl_view * ntl_trans * vec4(pos_in, 0.0, 1.0);
    vertColor = color_in;
}
)glsl";

const char *ntl::Shader::DefaultFragmentSource = R"glsl(
#version 330 core
in vec4 vertColor;
out vec4 color_out;

void main() {
    color_out = vertColor;
    // color_out = vec4(1, 0, 0, 1);
}
)glsl";

ntl::Shader::Shader(const char *vertexSrc, const char *fragmentSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);

    GLint status{};
    if (glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status); status != GL_TRUE) {
        char buffer[512]{};
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        throw std::runtime_error("Error to build vertex shader: " + std::string(buffer));
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);

    if (glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status); status != GL_TRUE) {
        char buffer[512]{};
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        throw std::runtime_error("Error to build fragment shader: " + std::string(buffer));
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);
    glLinkProgram(program_);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (glGetProgramiv(program_, GL_LINK_STATUS, &status); status != GL_TRUE) {
        char buffer[512]{};
        glGetProgramInfoLog(program_, 512, nullptr, buffer);
        throw std::runtime_error("Shader linking failed: " + std::string(buffer));
    }

    // glBindFragDataLocation(program_, 0, "color_out"); // Не даёт никакого эффекта
}

ntl::Shader::~Shader() {
    glDeleteProgram(program_);
}

void ntl::Shader::use() const {
    glUseProgram(program_);
}

void ntl::Shader::setUniform(std::string_view name, const Vector2f &value) const {
    GLint loc = glGetUniformLocation(program_, name.data());
    glUniform2f(loc, value.x, value.y);
}

void ntl::Shader::setUniform(std::string_view name, const Matrix4x4f &value) const {
    GLint loc = glGetUniformLocation(program_, name.data());
    glUniformMatrix4fv(loc, 1, GL_TRUE, value.data());
}