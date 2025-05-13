#ifndef GAME_GRID_HPP
#define GAME_GRID_HPP

#include "NTL/Graphics.hpp"

#include <array>
#include <vector>
#include "TextureManager.hpp"
#include "GraphicManager.hpp"
#include "Core.hpp"

struct GameGrid;

class GameGridDrawable : public ntl::Drawable {
public:
    GameGridDrawable(const GameGrid *grid) :
        grid(grid)
    {}

    GameGridDrawable(const GameGridDrawable &other) :
        grid(other.grid)
    {}

    ntl::Vector2f getPosition(ntl::Vector2u viewSize) const {
        return {
            (float)viewSize.x / 4.f - (float)Core::getCellsize() * 5.f,
            (float)viewSize.y / 2.f - (float)Core::getCellsize() * 5.f
        };
    }

public:
    const GameGrid *grid;

protected:
    void draw(ntl::Window &target) const override {
        const int GRID_WIDTH = 10;
        const int GRID_HEIGHT = 10;
        const int MARGIN = 50;
        auto cellSize = Core::getCellsize();

        ntl::RectangleShape cell(ntl::Vector2f(cellSize - 2, cellSize - 2));
        cell.setOutlineThickness(1);
        cell.setOutlineColor(ntl::Color::Black);

        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                cell.setPosition(getPosition(target.getSize()) + ntl::Vector2f(x, y) * cellSize);
                cell.setFillColor(ntl::Color(200, 230, 255));
                target.draw(cell, states);
            }
        }
    }
};

class GameGrid {
public:
    GameGrid(GraphicManager &graphics, int zLayer) :
        graphics(graphics),
        gridDrawable(graphics.add<GameGridDrawable>(zLayer, this))
    {}

    GameGrid(const Ship &other) :
        graphics(other.graphics),
        gridDrawable(gridDrawable),
        grid(grid)
    {}

    GameGrid(Ship &&other) :
        graphics(other.graphics),
        gridDrawable(gridDrawable),
        grid(std::move(grid))
    {}

public:
    GraphicManager &graphics;
    GameGridDrawable *gridDrawable = nullptr;
    std::array<std::array<bool, 10>, 10> grid{};

    static constexpr auto RECT = ntl::IntRect({0, 0}, {10, 10});
};

#endif // GAME_GRID_HPP