#ifndef NTL__SHAPE_HPP
#define NTL__SHAPE_HPP

#include <vector>
#include "../Utils/Rectangle.hpp"
#include "../Utils/Color.hpp"
#include "PrimitiveType.hpp"
#include "Vertex.hpp"
#include "Drawable.hpp"
#include "Transformable.hpp"

namespace ntl {

struct Texture;

class Shape : public Drawable, public Transformable {
public:
    Shape() = default;

    Shape(std::initializer_list<Vertex> vertices, PrimitiveType type) :
        vertices_(vertices.begin(), vertices.end()),
        type_(type)
    {}

    template<class Container>
    Shape(Container vertices, PrimitiveType type) :
        vertices_(vertices.begin(), vertices.end()),
        type_(type)
    {}


    void setVertices(std::initializer_list<Vertex> vertices, PrimitiveType type) {
        vertices_.assign(vertices.begin(), vertices.end());
        type_ = type;
    }

    template<class Container>
    void setVertices(Container vertices, PrimitiveType type) {
        vertices_.assign(vertices.begin(), vertices.end());
        type_ = type;
    }


    void setTexture(const Texture *texture) {
        texture_ = texture;
    }

    const Texture *getTexture() const {
        return texture_;
    }


    void setColor(Color color);

    FloatRect getLocalBounds() const;

    FloatRect getGlobalBounds() const;

protected:
    std::vector<Vertex> vertices_;
    PrimitiveType type_ = PrimitiveType::Points;
    const Texture *texture_{};

    virtual void draw(Window &window, RenderStates renderStates) const override;
};

}

#endif // NTL__SHAPE_HPP