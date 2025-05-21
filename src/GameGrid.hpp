#ifndef GAME_GRID_HPP
#define GAME_GRID_HPP

#include "NTL/Graphics.hpp"

#include <iostream>
#include <array>
#include <vector>
#include "GraphicManager.hpp"
#include "Core.hpp"
#include "Ship.hpp"

struct GameGrid;

class GameGridDrawable : public ntl::Drawable {
public:
    GameGridDrawable(const GameGrid *grid, bool inLeft) :
        grid(grid),
        inLeft(inLeft)
    {}

    GameGridDrawable(const GameGridDrawable &other) :
        grid(other.grid),
        inLeft(other.inLeft)
    {}

    ntl::Vector2f getPosition(ntl::Vector2i viewSize) const {
        return {
            inLeft
             ? ((float)viewSize.x * 0.25f - (float)Core::getCellsize() * 5.f)
             : ((float)viewSize.x * 0.75f - (float)Core::getCellsize() * 5.f),
            (float)viewSize.y / 2.f - (float)Core::getCellsize() * 5.f
        };
    }

    ntl::FloatRect getBounds(ntl::Vector2i viewSize) {
        return {
            getPosition(viewSize),
            {Core::getCellsize() * 10.f, Core::getCellsize() * 10.f}
        };
    }

public:
    const GameGrid *grid;
    bool inLeft{};

protected:
    void draw(ntl::Window &target, ntl::RenderStates states) const override;
};

class GameGrid {
public:
    GameGrid(GraphicManager &graphics, int zLayer, bool inLeft) :
        graphics(graphics),
        gridDrawable(graphics.add<GameGridDrawable>(zLayer, this, inLeft))
    {
        for (int i = 1; i <= 4; ++i) {
            for (int j = 0; j <= 4 - i; ++j) {
                ships.emplace_back(graphics, Core::LAYER_SHIP, i);
                // Positions will be set later
            }
        }
    }

    GameGrid(GameGrid &&other) :
        graphics(other.graphics),
        gridDrawable(other.gridDrawable),
        grid(other.grid),
        ships(std::move(other.ships))
    {
        gridDrawable->grid = this;
    }

    int touch(ntl::Vector2i viewSize, ntl::Vector2f pos) {
        const auto bounds = gridDrawable->getBounds(viewSize);
        if (bounds.contains(pos)) {
            const auto cellPos = static_cast<ntl::Vector2i>((pos - bounds.position) / Core::getCellsize());

            if (!grid[cellPos.x][cellPos.y]) {
                bool isTouchedShip = false;
                for (auto &ship : ships) {
                    if (ship.touch(cellPos)) {
                        if (ship.getState() == Ship::State::Defeated) {
                            const auto shipBounds = ship.getBounds(true);

                            for (int x = 0; x < shipBounds.size.x; ++x) {
                                for (int y = 0; y < shipBounds.size.y; ++y) {
                                    grid[shipBounds.position.x + x][shipBounds.position.y + y] = true;
                                }
                            }
                        }

                        isTouchedShip = true;
                        break;
                    }
                }

                grid[cellPos.x][cellPos.y] = true;
                return isTouchedShip ? 2 : 1;
            }
        }
        return 0;
    }

public:
    GraphicManager &graphics;
    GameGridDrawable *gridDrawable = nullptr;
    std::array<std::array<bool, 10>, 10> grid{};
    std::vector<Ship> ships;

    static constexpr auto RECT = ntl::IntRect({0, 0}, {10, 10});
};

#endif // GAME_GRID_HPP