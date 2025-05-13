#ifndef NTL__RECTANGULAR_HPP
#define NTL__RECTANGULAR_HPP

#include "Vector2.hpp"

namespace ntl {

template<class T>
class Rectangular {
public:
    constexpr Rectangular() = default;

    constexpr Rectangular(Vector2<T> position, Vector2<T> size)
        : position(position), size(size) {}

    template<class OtherT>
    constexpr explicit Rectangular(Rectangular<OtherT> other)
        : position((Vector2<OtherT>)position), size((Vector2<OtherT>)size) {}

    template<class OtherT>
    constexpr explicit operator Rectangular<OtherT>() {
        return { (Vector2<OtherT>)position, (Vector2<OtherT>)size };
    }


    constexpr bool contains(Vector2<T> point) const {
        return (point.x > position.x) && (point.y > position.y)
            && (point.x < position.x + size.x) && (point.y < position.y + size.y);
    }

    constexpr bool isIntersect(const Rectangular<T> &other) const {
        return (position.x < other.position.x + other.size.x)
            && (position.x + size.x > other.position.x)
            && (position.y < other.position.y + other.size.y)
            && (position.y + size.y > other.position.y);
    }

public:
    T position{};
    T size{};
};

using FloatRect = Rectangular<float>;
using IntRect = Rectangular<int>;

}

#endif // NTL__RECTANGULAR_HPP