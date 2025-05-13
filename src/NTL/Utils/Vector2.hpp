#ifndef NTL__VECTOR2_HPP
#define NTL__VECTOR2_HPP

namespace ntl {

template<class T>
class Vector2 {
public:
    constexpr Vector2() = default;

    constexpr Vector2(T x, T y)
        : x(x), y(y) {}

    template<class OtherT>
    constexpr explicit Vector2(Vector2<OtherT> other)
        : x((T)other.x), y((T)other.y) {}

    template<class OtherT>
    constexpr explicit operator Vector2<OtherT>() {
        return { (OtherT)x, (OtherT)y };
    }


    constexpr Vector2<T> &operator+=(Vector2<T> other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vector2<T> &operator-=(Vector2<T> other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vector2<T> operator+(Vector2<T> other) const {
        return {
            x + other.x,
            y + other.y,
        };
    }

    constexpr Vector2<T> operator-(Vector2<T> other) const {
        return {
            x - other.x,
            y - other.y,
        };
    }

    constexpr Vector2<T> operator-() const {
        return { -x, -y };
    }

public:
    T x{};
    T y{};
};

template<class T>
constexpr Vector2<T> operator*(Vector2<T> left, T right) {
    return {
        left.x * right,
        left.y * right,
    };
}

template<class T>
constexpr Vector2<T> operator*(T left, Vector2<T> right) {
    return {
        left * right.x,
        left * right.y,
    };
}

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

}

#endif // NTL__VECTOR2_HPP