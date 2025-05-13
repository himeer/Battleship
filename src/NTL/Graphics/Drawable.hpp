#ifndef NTL__DRAWABLE_HPP
#define NTL__DRAWABLE_HPP

namespace ntl {

class Window;

class Drawable {
public:
    virtual ~Drawable() = default;

protected:
    friend class Window;

    virtual void draw(Window &target) const = 0;
};

}

#endif // NTL__DRAWABLE_HPP