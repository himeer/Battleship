#ifndef SHIP_HPP
#define SHIP_HPP

#include "NTL/Graphics.hpp"

#include <vector>
#include "GraphicManager.hpp"
#include "Core.hpp"

struct Ship;

class ShipDrawable : public ntl::Drawable, public ntl::Transformable {
public:
    explicit ShipDrawable(const Ship *ship) :
        ship(ship)
    {
        setOrigin({Core::getCellsize() / 2.f, Core::getCellsize() / 2.f});
    }

    ShipDrawable(const ShipDrawable &other) :
        ship(other.ship)
    {}

    ntl::FloatRect getGlobalBounds() const;

public:
    const Ship *ship{};

protected:
    virtual void draw(ntl::Window &target, ntl::RenderStates states) const override;
};

class Ship {
public:
    enum class State {
        Safe,
        Damaged,
        Defeated,
    };

    Ship(GraphicManager &graphics, int zLayer, size_t rang) :
        graphics(graphics),
        segments(rang, true),
        cellPosition({-1, -1}),
        menuPosition({-1, -1})
    {
        drawable = graphics.add<ShipDrawable>(zLayer, this);
    }

    Ship(Ship &&other) :
        graphics(other.graphics),
        segments(std::move(other.segments)),
        vertical(other.vertical),
        cellPosition(other.cellPosition),
        menuPosition(other.menuPosition),
        drawable(other.drawable)
    {
        drawable->ship = this;
    }

    inline std::size_t rang() const {
        return segments.size();
    }

    State getState() const {
        int defeatedSegments = 0;
        for (auto segment : segments) defeatedSegments += !segment;

        return !defeatedSegments ? State::Safe
            : defeatedSegments == segments.size() ? State::Defeated
            : State::Damaged;
    }

    inline void rotate() {
        rotate(!vertical);
    }

    void rotate(bool vertical) {
        this->vertical = vertical;

        drawable->setRotation(ntl::degrees(vertical ? 0.f : 90.f));
    }

    bool setLayer(int zLayer) {
        return graphics.setLayer(drawable, zLayer);
    }

    ntl::IntRect getBounds(bool withExternal) const {
        ntl::IntRect bounds(cellPosition - ntl::Vector2i((int)withExternal, (int)withExternal), {
            int(vertical ? 1 : rang()) + 2 * (int)withExternal,
            int(vertical ? rang() : 1) + 2 * (int)withExternal,
        });

        return *bounds.getIntersection({{0, 0}, {10, 10}});
    }

    bool isIntersect(const Ship &other) const {
        if (cellPosition.x < 0 || other.cellPosition.x < 0) return false;

        return getBounds(true).isIntersect(other.getBounds(false));
    }

    bool touch(ntl::Vector2i cellPos) {
        auto bounds = getBounds(false);
        bounds.size -= {1, 1};

        if (bounds.contains(cellPos)) {
            const auto shipDelta = cellPos - bounds.position;
            const auto segmentIdx = vertical ? shipDelta.y : shipDelta.x;

            if (segmentIdx < segments.size()) {
                segments[segmentIdx] = false;
                return true;
            }
        }

        return false;
    }

public:
    GraphicManager &graphics;
    std::vector<bool> segments;
    bool vertical = true;
    ntl::Vector2i cellPosition;
    ntl::Vector2f menuPosition;
    ShipDrawable *drawable = nullptr;
};

#endif // SHIP_HPP