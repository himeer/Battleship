#ifndef NTL__RECTANGLE_SHAPE_HPP
#define NTL__RECTANGLE_SHAPE_HPP

#include "Shape.hpp"

namespace ntl {

class Window;

class RectangleShape : public Shape {
public:
    RectangleShape(ntl::Vector2f size, ntl::Color color, PrimitiveType type = PrimitiveType::TriangleFan) :
        Shape({
            {{0.f, 0.f}, color},
            {{size.x, 0.f}, color},
            {size, color},
            {{0.f, size.y}, color},
            {{0.f, 0.f}, color},
        }, type)
    {}

private:
    using Shape::setVertices;
};

}

#endif // NTL__RECTANGLE_SHAPE_HPP