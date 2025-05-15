#ifndef NTL__TRANSFORMABLE_HPP
#define NTL__TRANSFORMABLE_HPP

#include "../Utils/Matrix.hpp"

namespace ntl {

class Transformable {
public:
    Transformable() = default;

    virtual ~Transformable() = default;


    void setPosition(Vector2f position) {
        isNeedUpdateTransform_ = true;
        position_ = position;
    }

    void setOrigin(Vector2f origin) {
        isNeedUpdateTransform_ = true;
        origin_ = origin;
    }

    void setRotation(Angle angle) {
        isNeedUpdateTransform_ = true;
        rotation_ = angle;
    }

    void setScale(Vector2f scale) {
        isNeedUpdateTransform_ = true;
        scale_ = scale;
    }


    Vector2f getPosition() const {
        return position_;
    }

    Vector2f getOrigin() const {
        return origin_;
    }

    Angle getRotation() const {
        return rotation_;
    }

    Vector2f getScale() const {
        return scale_;
    }


    const Matrix4x4f &getTransform() const {
        if (isNeedUpdateTransform_) {
            const float angle = rotation_.asRadians();
            const float cos = std::cos(angle);
            const float sin = std::sin(angle);
            const float sxc = scale_.x * cos;
            const float syc = scale_.y * cos;
            const float sxs = scale_.x * sin;
            const float sys = scale_.y * sin;
            const float tx = -origin_.x * sxc - origin_.y * sys + position_.x;
            const float ty =  origin_.x * sxs - origin_.y * syc + position_.y;

            transform_ = Matrix4x4f{
                 sxc, sys, 0, tx,
                -sxs, syc, 0, ty,
                 0,   0,   1, 0,
                 0,   0,   0, 1,
            };
            isNeedUpdateTransform_ = false;
        }

        return transform_;
    }

private:
    Vector2f origin_;
    Vector2f position_;
    Angle rotation_;
    Vector2f scale_{1, 1};

    mutable Matrix4x4f transform_;
    mutable bool isNeedUpdateTransform_ = true;
};

}

#endif // NTL__TRANSFORMABLE_HPP