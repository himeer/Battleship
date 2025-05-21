#ifndef NTL__SPRITE_HPP
#define NTL__SPRITE_HPP

#include "Shape.hpp"
#include "Texture.hpp"

namespace ntl {

class Sprite : public Shape {
public:
    Sprite(const Texture &texture) {
        const auto [w, h] = (Vector2f)texture.getSize();

        setTexture(&texture);
        setVertices({
            {{0, 0}, Color::White, {0, 0}},
            {{0, h}, Color::White, {0, 1}},
            {{w, h}, Color::White, {1, 1}},
            {{w, 0}, Color::White, {1, 0}},
        }, PrimitiveType::TriangleFan);
    }

private:
    using Shape::setVertices;
};

}

#endif // NTL__SPRITE_HPP