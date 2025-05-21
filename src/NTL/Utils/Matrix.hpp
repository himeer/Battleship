#ifndef NTL__MATRIX_HPP
#define NTL__MATRIX_HPP

#include <array>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

#include "Angle.hpp"
#include "Vector2.hpp"
#include "Rectangle.hpp"

namespace ntl {

template<class T>
class Matrix4x4 {
public:
    constexpr Matrix4x4() :
        data_({
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        })
    {}

    constexpr Matrix4x4(std::initializer_list<T> data) {
        if (data.size() != 16) {
            throw std::runtime_error("Matrix4x4 must be initialized by list with 16 values");
        }
        std::copy(data.begin(), data.end(), data_.begin());
    }

    constexpr Matrix4x4(const Matrix4x4<T> &other) = default;

    constexpr Matrix4x4<T> &operator=(const Matrix4x4<T> &other) = default;


    constexpr T *operator[](std::size_t i) {
        return data_.data() + i * 4;
    }

    constexpr const T *operator[](std::size_t i) const {
        return data_.data() + i * 4;
    }

    constexpr T *data() {
        return data_.data();
    }

    constexpr const T *data() const {
        return data_.data();
    }


    constexpr Matrix4x4<T> operator*(const Matrix4x4<T> &other) const {
        Matrix4x4<T> out;

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                out[i][j] = 0;

                for (int k = 0; k < 4; ++k) {
                    out[i][j] += (*this)[i][k] * other[k][j];
                }
            }
        }

        return out;
    }

    constexpr Matrix4x4<T> &operator*=(const Matrix4x4<T> &other) {
        return *this = *this * other;
    }


    void rotateAroundZ(Angle angle) {
        auto cos = static_cast<T>(std::cos(angle.asRadians()));
        auto sin = static_cast<T>(std::sin(angle.asRadians()));

        *this *= Matrix4x4({
            cos, -sin, 0, 0,
            sin,  cos, 0, 0,
            0,    0,   1, 0,
            0,    0,   0, 1,
        });
    }

    constexpr void translate(Vector2<T> delta) {
        *this *= Matrix4x4({
            1, 0, 0, delta.x,
            0, 1, 0, delta.y,
            0, 0, 1, 0,
            0, 0, 0, 1,
        });
    }

    constexpr void scale(Vector2<T> scale) {
        auto [x, y] = scale;

        *this *= Matrix4x4({
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        });
    }

    constexpr FloatRect transformRect(const FloatRect &rectangle) const;

public:
    static const Matrix4x4<T> Identity;

private:
    std::array<T, 16> data_{};
};

template<class T>
const Matrix4x4<T> Matrix4x4<T>::Identity;

using Matrix4x4f = Matrix4x4<float>;


template<class T>
constexpr Vector2<T> operator*(const Matrix4x4<T> &matrix, Vector2<T> vector) {
    auto [x, y] = vector;
    auto &m = matrix;
    return {
        m[0][0] * x + m[0][1] * y + m[0][3],
        m[1][0] * x + m[1][1] * y + m[1][3],
    };
}


template<class T>
constexpr FloatRect Matrix4x4<T>::transformRect(const FloatRect &rectangle) const {
    std::array points{
        rectangle.position,
        rectangle.position + Vector2f(0.f, rectangle.size.y),
        rectangle.position + rectangle.size,
        rectangle.position + Vector2f(rectangle.size.x, 0.f),
    };

    ntl::Vector2f topLeft = *this * points[0];
    ntl::Vector2f bottomRight = topLeft;

    for (auto &point : points) {
        const auto position = *this * point;

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

}

#endif // NTL__MATRIX_HPP