#ifndef NTL__WINDOW_HPP
#define NTL__WINDOW_HPP

#include <vector>
#include <optional>
#include <string_view>

#include "../Utils/Color.hpp"
#include "../Utils/Vector2.hpp"
#include "../Graphics/PrimitiveType.hpp"
#include "Event.hpp"

struct GLFWwindow;

namespace ntl {

struct Vertex;
struct Drawable;

class Window { // Now allow only single window
public:
    Window(Vector2i size, std::string_view title);

    ~Window();


    void close();

    bool isOpen() const;


    void display();

    void clear(Color color = Color::Black);

    void draw(const Drawable &drawable);

    void draw(const Vertex *vertices, size_t count, PrimitiveType type);


    std::optional<Event> pollEvent() const;

private:
    GLFWwindow *handle_{};
};

}

#endif // NTL__WINDOW_HPP