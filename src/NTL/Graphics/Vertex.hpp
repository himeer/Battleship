#ifndef NTL__VERTEX_HPP
#define NTL__VERTEX_HPP

#include <vector>
#include "../Utils/Vector2.hpp"
#include "../Utils/Color.hpp"
#include "PrimitiveType.hpp"

namespace ntl {

struct Vertex {
    Vector2f position;
    Color color = Color::White;
};

}

#endif // NTL__VERTEX_HPP