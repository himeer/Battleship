#include "GameGrid.hpp"
#include <algorithm>

void GameGridDrawable::draw(ntl::Window &target, ntl::RenderStates states) const {
    const auto cellSize = Core::getCellsize();

    ntl::RectangleShape cellOutline(
        ntl::Vector2f(cellSize, cellSize),
        ntl::Color::Black,
        ntl::PrimitiveType::LineStrip
    );

    ntl::RectangleShape cellDestroyed(
        ntl::Vector2f(cellSize / 4.f, cellSize / 4.f),
        ntl::Color::Black,
        ntl::PrimitiveType::TriangleFan
    );
    cellDestroyed.setOrigin(cellDestroyed.getLocalBounds().size / 2.f);
    cellDestroyed.setRotation(ntl::degrees(45.f));

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

            auto foundShip = std::any_of(grid->ships.begin(), grid->ships.end(),
                [&](const Ship &ship) {
                    auto bounds = ship.getBounds(false);
                    bounds.size -= {1, 1};
                    return bounds.contains(ntl::Vector2i{x, y});
                });

            if (grid->grid[x][y] && !foundShip) {
                cellDestroyed.setPosition(cellOutline.getPosition() + ntl::Vector2f(cellSize, cellSize) / 2.f);
                target.draw(cellDestroyed, states);
            }
        }
    }
}