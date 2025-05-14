#ifndef NTL__MATRIX_HPP
#define NTL__MATRIX_HPP

#include <array>

namespace ntl {

template<class T>
class Matrix4x4 {
public:
    constexpr Matrix4x4() {
        for (int i = 0; i < 4; ++i) data_[i * 4 + i] = 1;
    }

public:
    std::array<T, 16> data_{};
};

using Matrix4x4f = Matrix4x4<float>;

}

#endif // NTL__MATRIX_HPP