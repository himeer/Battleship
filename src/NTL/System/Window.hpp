#ifndef NTL__WINDOW_HPP
#define NTL__WINDOW_HPP

#include <vector>
#include <optional>
#include <string_view>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Utils/Color.hpp"
#include "../Utils/Vector2.hpp"
#include "../Utils/Matrix.hpp"
#include "../Utils/Rectangle.hpp"
#include "../Graphics/PrimitiveType.hpp"
#include "../Graphics/RenderStates.hpp"
#include "../Graphics/Shader.hpp"
#include "Event.hpp"

struct GLFWwindow;

namespace ntl {

struct Vertex;
struct Drawable;
struct VertexBuffer;

class Window { // Now allow only single window
public:
    Window(Vector2i size, std::string_view title);

    ~Window();


    void close();

    bool isOpen() const;


    Vector2i getSize() const;


    void setView(const Matrix4x4f &transform);

    Shader &getDefaultShader() {
        return defaultShader_;
    }


    void display();

    void clear(Color color = Color::Black);

    void draw(const Drawable &drawable, RenderStates renderStates = Matrix4x4f::Identity);

    void draw(const Vertex *vertices, size_t count, PrimitiveType type, RenderStates renderStates = Matrix4x4f::Identity);


    std::optional<Event> pollEvent() const;

private:
    GLFWwindow *handle_{};
    GLuint vao_{};
    GLuint vbo_{};
    size_t vboSize_{};
    Matrix4x4f view_;
    Shader defaultShader_;
};

}

#endif // NTL__WINDOW_HPP