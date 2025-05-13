#ifndef SHIP_HPP
#define SHIP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureManager.hpp"
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
        sprite = graphics.add<ntl::Sprite>(zLayer, TextureManager::getInstance()[getTextureId(rang)]);

        sprite->setScale({
            Core::getCellsize() / sprite->getTexture().getSize().x,
            Core::getCellsize() * rang / sprite->getTexture().getSize().y
        });
        sprite->setOrigin({
            sprite->getTexture().getSize().x / 2.f,
            sprite->getTexture().getSize().y / rang / 2.f
        });
    }

    Ship(const Ship &other) :
        graphics(other.graphics),
        segments(other.segments),
        vertical(other.vertical),
        areaPosition(other.areaPosition),
        menuPosition(other.menuPosition),
        sprite(other.sprite)
    {}

    Ship(Ship &&other) :
        graphics(other.graphics),
        segments(std::move(other.segments)),
        vertical(other.vertical),
        areaPosition(other.areaPosition),
        menuPosition(other.menuPosition),
        sprite(other.sprite)
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

        sprite->setRotation(ntl::degrees(vertical ? 0.f : -90.f));
    }

    bool setLayer(int zLayer) {
        return graphics.setLayer(sprite, zLayer);
    }

    ntl::IntRect getBounds(bool withExternal) const {
        return ntl::IntRect(areaPosition - ntl::Vector2i((int)withExternal, (int)withExternal), {
            int(vertical ? 1 : rang()) + 2 * (int)withExternal,
            int(vertical ? rang() : 1) + 2 * (int)withExternal,
        });
    }

    bool isIntersect(const Ship &other) const {
        if (areaPosition.x < 0 || other.areaPosition.x < 0) return false;

        return !!getBounds(true).findIntersection(other.getBounds(false));
    }

public:
    GraphicManager &graphics;
    std::vector<bool> segments;
    bool vertical = true;
    ntl::Vector2i areaPosition;
    ntl::Vector2f menuPosition;
    ntl::Sprite *sprite = nullptr;

protected:
    static TextureManager::ID getTextureId(size_t rang) {
        switch (rang) {
            case 1: return TextureManager::ID::Ship1; break;
            case 2: return TextureManager::ID::Ship2; break;
            case 3: return TextureManager::ID::Ship3; break;
            case 4: return TextureManager::ID::Ship4; break;
        }

        throw std::runtime_error("Invalid rang of ship: " + std::to_string(rang));
    }
};

#endif // SHIP_HPP