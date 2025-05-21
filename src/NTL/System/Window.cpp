#include "Window.hpp"
#include "../Graphics/Drawable.hpp"
#include "../Graphics/Vertex.hpp"

#include <cassert>
#include <queue>

namespace {

std::queue<ntl::Event> eventQueue;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int modes) {
    if (action == GLFW_PRESS) {
        eventQueue.emplace(ntl::Event::KeyPressed{
            .code = key,
            .scancode = scancode,
            .modes = modes,
        });
    } else if (action == GLFW_RELEASE) {
        eventQueue.emplace(ntl::Event::KeyReleased{
            .code = key,
            .scancode = scancode,
            .modes = modes,
        });
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    eventQueue.emplace(ntl::Event::MouseMoved{
        .position = {(int)xpos, (int)ypos},
    });
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int modes) {
    double xpos{}, ypos{};
    glfwGetCursorPos(window, &xpos, &ypos);

    if (action == GLFW_PRESS) {
        eventQueue.emplace(ntl::Event::MouseButtonPressed{
            .position = {(int)xpos, (int)ypos},
            .button = button,
            .modes = modes,
        });
    } else if (action == GLFW_RELEASE) {
        eventQueue.emplace(ntl::Event::MouseButtonReleased{
            .position = {(int)xpos, (int)ypos},
            .button = button,
            .modes = modes,
        });
    }
}

}

ntl::Window::Window(Vector2i size, std::string_view title) :
    defaultShader_(nullptr)
{
    assert(glfwInit());

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    handle_ = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
    assert(handle_);

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(1);

    glfwSetKeyCallback(handle_, key_callback);
    glfwSetCursorPosCallback(handle_, cursor_position_callback);
    glfwSetMouseButtonCallback(handle_, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }


    {
        float sx =  2.f / size.x;
        float sy = -2.f / size.y;
        view_ = Matrix4x4f{
            sx, 0,  0, -1,
            0,  sy, 0,  1,
            0,  0,  1,  0,
            0,  0,  0,  1
        };
    }

    // Create default shader after opengl init
    defaultShader_ = Shader();

    // Create VAO and VBO
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

ntl::Window::~Window() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glfwDestroyWindow(handle_);
    glfwTerminate();
}

void ntl::Window::close() {
    glfwSetWindowShouldClose(handle_, GLFW_TRUE);
}

bool ntl::Window::isOpen() const {
    return !glfwWindowShouldClose(handle_);
}

ntl::Vector2i ntl::Window::getSize() const {
    int width{}, height{};
    glfwGetWindowSize(handle_, &width, &height);
    return {width, height};
}

void ntl::Window::setView(const Matrix4x4f &transform) {
    view_ = transform;
}

void ntl::Window::display() {
    glfwSwapBuffers(handle_);
}

void ntl::Window::clear(Color color) {
    glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ntl::Window::draw(const Drawable &drawable, RenderStates renderStates) {
    drawable.draw(*this, renderStates);
}

void ntl::Window::draw(const Vertex *vertices, size_t count, PrimitiveType type, RenderStates renderStates) {
    if (!vertices || !count) {
        throw std::runtime_error("Cannot draw an empty vertices array");
    }

    static constexpr GLenum modes[] = {
        GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
    };
    const GLenum mode = modes[static_cast<size_t>(type)];

    if (!renderStates.shader) renderStates.shader = &defaultShader_;
    renderStates.shader->use();
    renderStates.shader->setUniform("ntl_view", view_);
    renderStates.shader->setUniform("ntl_trans", renderStates.transform);
    renderStates.shader->setUniform("ntl_useTexture", !!renderStates.texture);

    if (renderStates.texture) {
        renderStates.shader->setTexture("ntl_texture", *renderStates.texture, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    if (vboSize_ >= count * sizeof(Vertex)) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Vertex), vertices);
    } else {
        vboSize_ = count * sizeof(Vertex);
        glBufferData(GL_ARRAY_BUFFER, vboSize_, vertices, GL_DYNAMIC_DRAW);
    }

    glBindVertexArray(vao_);
    glDrawArrays(mode, 0, count);

    glBindVertexArray(0);
    glUseProgram(0);

    if (GLenum error = glGetError(); error != GL_NO_ERROR) {
        throw std::runtime_error("OpenGL error: " + std::to_string(error));
    }
}

std::optional<ntl::Event> ntl::Window::pollEvent() const {
    glfwPollEvents();

    if (!eventQueue.empty()) {
        std::optional<Event> event(eventQueue.front());
        eventQueue.pop();
        return event;
    } else {
        return {};
    }
}