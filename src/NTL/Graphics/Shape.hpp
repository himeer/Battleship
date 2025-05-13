#ifndef NTL__SHAPE_HPP
#define NTL__SHAPE_HPP

#include <vector>
#include "../Utils/Rectangular.hpp"
#include "../Utils/Color.hpp"
#include "PrimitiveType.hpp"
#include "Vertex.hpp"
#include "Drawable.hpp"

namespace ntl {

class Shape : Drawable {
public:
    void setFillColor(Color color);

    void setOutlineColor(Color color);

    void setOutlineThickness(float thickness);


    Color getFillColor() const;

    Color getOutlineColor() const;

    float getOutlineThickness() const;

    FloatRect getBounds() const;

protected:
    Color fillColor_;
    Color outlineColor_;
    float outlineThickness_;
    std::vector<Vertex> vertices_;
    std::vector<Vertex> verticesOutline_;

private:
    virtual void draw(Window &window) const override;
};

}

#endif // NTL__SHAPE_HPP