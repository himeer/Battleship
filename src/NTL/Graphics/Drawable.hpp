#ifndef NTL__DRAWABLE_HPP
#define NTL__DRAWABLE_HPP

#include "RenderStates.hpp"

namespace ntl {

class Window;

class Drawable {
public:
    virtual ~Drawable() = default;

protected:
    friend class Window;

    virtual void draw(Window &target, RenderStates renderStates) const = 0;
};

}

#endif // NTL__DRAWABLE_HPP