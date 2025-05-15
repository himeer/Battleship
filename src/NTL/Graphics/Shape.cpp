#include "Shape.hpp"
#include "../System/Window.hpp"

void ntl::Shape::setColor(Color color) {
    for (auto &vertex : vertices_) vertex.color = color;
}

ntl::FloatRect ntl::Shape::getBounds() const {
    if (vertices_.empty()) {
        throw std::runtime_error("Shape doesn't has vertices!");
    }

    ntl::Vector2f topLeft = getTransform() * vertices_[0].position;
    ntl::Vector2f bottomRight = topLeft;

    for (auto &vertex : vertices_) {
        auto position = getTransform() * vertex.position;

        topLeft.x = std::min(topLeft.x, position.x);
        topLeft.y = std::min(topLeft.y, position.y);
        bottomRight.x = std::max(bottomRight.x, position.x);
        bottomRight.y = std::max(bottomRight.y, position.y);
    }

    return {
        topLeft,
        bottomRight - topLeft
    };
}

void ntl::Shape::draw(Window &window, RenderStates renderStates) const {
    auto transform = *renderStates.transform * getTransform();
    renderStates.transform = &transform;
    window.draw(vertices_.data(), vertices_.size(), type_, renderStates);
}
