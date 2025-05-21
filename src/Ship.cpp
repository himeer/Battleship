#include "Ship.hpp"
#include <numbers>

ntl::FloatRect ShipDrawable::getGlobalBounds() const {
    return getTransform().transformRect({
        {0.f, 0.f}, {Core::getCellsize(), Core::getCellsize() * ship->rang()}
    });
}

void ShipDrawable::draw(ntl::Window &target, ntl::RenderStates states) const {
    const float cs = Core::getCellsize();
    const ntl::Color color(0x004455ff);
    const auto state = ship->getState();
    const auto rang = ship->rang();

    states.transform *= getTransform();

    if (ship->menuPosition.x >= 0 || state == Ship::State::Defeated) {
        ntl::Shape shape({
            {{cs / 2.f, 0.f      }, color},
            {{cs,       cs / 2.f }, color},
            {{cs,       cs * rang}, color},
            {{0.f,      cs * rang}, color},
            {{0.f,      cs / 2.f }, color},
        }, ntl::PrimitiveType::TriangleFan);

        target.draw(shape, states);
    } else if (state == Ship::State::Damaged) {
        ntl::RectangleShape line({2.f, cs * std::numbers::sqrt2_v<float> - 6.f}, ntl::Color::Red);
        line.setOrigin(line.getLocalBounds().size / 2.f);

        for (int i = 0; i < ship->segments.size(); ++i) {
            if (!ship->segments[i]) {
                line.setPosition(ntl::Vector2f{cs / 2.f, cs * ((float)i + 0.5f)});

                line.setRotation(ntl::degrees(45.f));
                target.draw(line, states);

                line.setRotation(ntl::degrees(-45.f));
                target.draw(line, states);
            }
        }
    }

}