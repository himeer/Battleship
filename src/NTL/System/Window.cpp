#include "Window.hpp"
#include "../Graphics/Drawable.hpp"
#include "../Graphics/Vertex.hpp"

#include <GLFW/glfw3.h>
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
    } if (action == GLFW_RELEASE) {
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
    } if (action == GLFW_RELEASE) {
        eventQueue.emplace(ntl::Event::MouseButtonReleased{
            .position = {(int)xpos, (int)ypos},
            .button = button,
            .modes = modes,
        });
    }
}

}

ntl::Window::Window(Vector2i size, std::string_view title) {
    assert(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle_ = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
    assert(handle_);

    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(1);

    glfwSetKeyCallback(handle_, key_callback);
    glfwSetCursorPosCallback(handle_, cursor_position_callback);
    glfwSetMouseButtonCallback(handle_, mouse_button_callback);
}

ntl::Window::~Window() {
    glfwDestroyWindow(handle_);
    glfwTerminate();
}

void ntl::Window::close() {
    glfwSetWindowShouldClose(handle_, GLFW_TRUE);
}

bool ntl::Window::isOpen() const {
    return !glfwWindowShouldClose(handle_);
}

void ntl::Window::display() {
    glfwSwapBuffers(handle_);
}

void ntl::Window::clear(Color color) {
    // TODO: очищяет экран
}

void ntl::Window::draw(const Drawable &drawable) {
    drawable.draw(*this);
}

void ntl::Window::draw(const Vertex *vertices, size_t count, PrimitiveType type) {
    glfwMakeContextCurrent(handle_);

    const auto* data = reinterpret_cast<const std::byte*>(vertices);

    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + offsetof(Vertex, position));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + offsetof(Vertex, color));

    static constexpr GLenum modes[] = {
        GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
    };
    const GLenum mode = modes[(size_t)type];

    glDrawArrays(mode, static_cast<GLint>(0), static_cast<GLsizei>(count));

    // TODO: Нужно доделать. Это только часть непротестированного кода. Можно взять его за основу
}

std::optional<ntl::Event> ntl::Window::pollEvent() const {
    glfwPollEvents();

    if (!eventQueue.empty()) {
        std::optional<Event> event(eventQueue.back());
        eventQueue.pop();
        return event;
    } else {
        return {};
    }
}