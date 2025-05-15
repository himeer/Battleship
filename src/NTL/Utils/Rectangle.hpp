#ifndef NTL__RECTANGLE_HPP
#define NTL__RECTANGLE_HPP

#include "Vector2.hpp"

namespace ntl {

template<class T>
class Rectangle {
public:
    constexpr Rectangle() = default;

    constexpr Rectangle(Vector2<T> position, Vector2<T> size)
        : position(position), size(size) {}

    template<class OtherT>
    constexpr explicit Rectangle(Rectangle<OtherT> other)
        : position((Vector2<OtherT>)position), size((Vector2<OtherT>)size) {}

    template<class OtherT>
    constexpr explicit operator Rectangle<OtherT>() {
        return { (Vector2<OtherT>)position, (Vector2<OtherT>)size };
    }


    constexpr bool contains(Vector2<T> point) const {
        return (point.x > position.x) && (point.y > position.y)
            && (point.x < position.x + size.x) && (point.y < position.y + size.y);
    }

    constexpr bool isIntersect(const Rectangle<T> &other) const {
        return (position.x < other.position.x + other.size.x)
            && (position.x + size.x > other.position.x)
            && (position.y < other.position.y + other.size.y)
            && (position.y + size.y > other.position.y);
    }

public:
    Vector2<T> position{};
    Vector2<T> size{};
};

using FloatRect = Rectangle<float>;
using IntRect = Rectangle<int>;

}

#endif // NTL__RECTANGLE_HPP