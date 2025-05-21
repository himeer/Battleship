#ifndef NTL__ANGLE_HPP
#define NTL__ANGLE_HPP

#include <numbers>

namespace ntl {

class Angle {
public:
    constexpr Angle() = default;

    constexpr float asDegrees() const {
        return radians_ * (180.f / std::numbers::pi_v<float>);
    }

    constexpr float asRadians() const {
        return radians_;
    }

private:
    float radians_ = 0.f;

    friend constexpr Angle degrees(float angle);
    friend constexpr Angle radians(float angle);

    constexpr explicit Angle(float radians)
        : radians_(radians) {}
};

constexpr Angle degrees(float angle) {
    return Angle(angle * (std::numbers::pi_v<float> / 180.f));
}

constexpr Angle radians(float angle) {
    return Angle(angle);
}

}

#endif // NTL__ANGLE_HPP