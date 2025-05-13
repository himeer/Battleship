#ifndef NTL__COLOR_HPP
#define NTL__COLOR_HPP

#include <cstdint>

namespace ntl {

class Color {
public:
    constexpr Color() = default;

    constexpr explicit Color(uint32_t bytes) :
        r((bytes & 0xFF000000u) >> 24),
        g((bytes & 0x00FF0000u) >> 16),
        b((bytes & 0x0000FF00u) >> 8),
        a((bytes & 0x000000FFu) >> 0)
    {}

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}

public:
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a{};

    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Transparent;
};

}

#endif // NTL__COLOR_HPP