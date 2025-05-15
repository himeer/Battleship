#ifndef SHIP_HPP
#define SHIP_HPP

#include "NTL/Graphics.hpp"

#include <vector>
#include "GraphicManager.hpp"
#include "Core.hpp"

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
        areaPosition({-1, -1})
    {
        const float cs = Core::getCellsize();
        const ntl::Color color(0x004455ff);

        shape = graphics.add<ntl::Shape>(zLayer, std::initializer_list<ntl::Vertex>{
            {{cs / 2.f, 0.f      }, color},
            {{cs,       cs / 2.f }, color},
            {{cs,       cs * rang}, color},
            {{0.f,      cs * rang}, color},
            {{0.f,      cs / 2.f }, color},
        }, ntl::PrimitiveType::TriangleFan);

        shape->setOrigin({cs / 2.f, cs / 2.f});
    }

    Ship(const Ship &other) :
        graphics(other.graphics),
        segments(other.segments),
        vertical(other.vertical),
        areaPosition(other.areaPosition),
        menuPosition(other.menuPosition),
        shape(other.shape)
    {}

    Ship(Ship &&other) :
        graphics(other.graphics),
        segments(std::move(other.segments)),
        vertical(other.vertical),
        areaPosition(other.areaPosition),
        menuPosition(other.menuPosition),
        shape(other.shape)
    {}

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

        shape->setRotation(ntl::degrees(vertical ? 0.f : 90.f));
    }

    bool setLayer(int zLayer) {
        return graphics.setLayer(shape, zLayer);
    }

    ntl::IntRect getBounds(bool withExternal) const {
        return ntl::IntRect(areaPosition - ntl::Vector2i((int)withExternal, (int)withExternal), {
            int(vertical ? 1 : rang()) + 2 * (int)withExternal,
            int(vertical ? rang() : 1) + 2 * (int)withExternal,
        });
    }

    bool isIntersect(const Ship &other) const {
        if (areaPosition.x < 0 || other.areaPosition.x < 0) return false;

        return getBounds(true).isIntersect(other.getBounds(false));
    }

public:
    GraphicManager &graphics;
    std::vector<bool> segments;
    bool vertical = true;
    ntl::Vector2i areaPosition;
    ntl::Vector2f menuPosition;
    ntl::Shape *shape = nullptr;
};

#endif // SHIP_HPP