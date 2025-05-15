#ifndef GAME_GRID_HPP
#define GAME_GRID_HPP

#include "NTL/Graphics.hpp"

#include <array>
#include <vector>
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

    ntl::Vector2f getPosition(ntl::Vector2i viewSize) const {
        return {
            (float)viewSize.x / 4.f - (float)Core::getCellsize() * 5.f,
            (float)viewSize.y / 2.f - (float)Core::getCellsize() * 5.f
        };
    }

public:
    const GameGrid *grid;

protected:
    void draw(ntl::Window &target, ntl::RenderStates states) const override {
        auto cellSize = Core::getCellsize();

        ntl::RectangleShape cellOutline(
            ntl::Vector2f(cellSize, cellSize),
            ntl::Color::Black,
            ntl::PrimitiveType::LineStrip
        );

        ntl::RectangleShape areaBackground(
            ntl::Vector2f(cellSize * 10, cellSize * 10),
            ntl::Color(200, 230, 255),
            ntl::PrimitiveType::TriangleFan
        );
        areaBackground.setPosition(getPosition(target.getSize()));
        target.draw(areaBackground, states);

        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                cellOutline.setPosition(getPosition(target.getSize()) + ntl::Vector2f(x, y) * cellSize);
                target.draw(cellOutline, states);
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