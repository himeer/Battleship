#include "ShipPlacementMenu.hpp"
#include "Core.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

ShipPlacementMenu::ShipPlacementMenu(ntl::Window &window) :
    App(window, ntl::Color(0x7aa1cdff)),
    grid(graphics, LAYER_GRID)
{
    divider = graphics.add<ntl::RectangleShape>(
        0,
        ntl::Vector2f(3.f, (float)window.getSize().y),
        ntl::Color(0x152b44ff)
    );
    divider->setPosition({float(window.getSize().x / 2), 0.f});

    for (int i = 1; i <= 4; ++i) {
        for (int j = 0; j <= 4 - i; ++j) {
            ships.emplace_back(graphics, LAYER_SHIP, i);
            auto &ship = ships.back();
            ship.rotate(false);
            ship.shape->setPosition({
                float(window.getSize().x / 2) + float(j * (i + 1) + 1) * Core::getCellsize() + ship.shape->getOrigin().x,
                float(i * 2 - 1) * Core::getCellsize() + ship.shape->getOrigin().y,
            });
            ship.menuPosition = ship.shape->getPosition();
        }
    }
}

bool ShipPlacementMenu::handle(const ntl::Event &event) {
    if (const auto* keyPressed = event.getIf<ntl::Event::KeyPressed>()) {
        if (keyPressed->code == GLFW_KEY_R && selectedShipIdx >= 0) {
            ships[selectedShipIdx].rotate();
        }
    } else if (const auto* mousePressed = event.getIf<ntl::Event::MouseButtonPressed>()) {
        if (mousePressed->button == GLFW_MOUSE_BUTTON_LEFT) {
            if (auto touched = graphics.getTouched((ntl::Vector2f)mousePressed->position)) {
                selectedShipIdx = -1;

                for (int i = 0; i < ships.size(); ++i) {
                    if (ships[i].shape == touched->first) selectedShipIdx = i;
                }

                if (selectedShipIdx >= 0) ships[selectedShipIdx].setLayer(LAYER_SHIP_ACTIVE);
            }
        }
    } else if (const auto* mouseReleased = event.getIf<ntl::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == GLFW_MOUSE_BUTTON_LEFT && selectedShipIdx >= 0) {
            auto &selectedShip = ships[selectedShipIdx];
            auto gridPos = grid.gridDrawable->getPosition(window.getSize());
            auto cellPos = (selectedShip.shape->getPosition() - gridPos) / Core::getCellsize();

            selectedShip.areaPosition = (ntl::Vector2i)cellPos;
            auto shipBounds = selectedShip.getBounds(false);

            if (
                ((ntl::FloatRect)GameGrid::RECT).contains(cellPos)
                && GameGrid::RECT.contains(shipBounds.position)
                && GameGrid::RECT.contains(shipBounds.position + shipBounds.size)
                && std::all_of(ships.begin(), ships.end(), [&](const Ship &ship) {
                    return &ship == &selectedShip || !ship.isIntersect(selectedShip);
                })
            ) {
                ntl::Vector2f cellPosTrunc(std::trunc(cellPos.x), std::trunc(cellPos.y));

                selectedShip.shape->setPosition(
                    gridPos + cellPosTrunc * Core::getCellsize() + selectedShip.shape->getOrigin().componentWiseMul(selectedShip.shape->getScale())
                );
            } else {
                selectedShip.areaPosition = {-1, -1};
                selectedShip.shape->setPosition(selectedShip.menuPosition);
                selectedShip.rotate(false);
            }

            selectedShip.setLayer(LAYER_SHIP);
            selectedShipIdx = -1;
        }
    } else if (const auto* mouseMoved = event.getIf<ntl::Event::MouseMoved>()) {
        if (selectedShipIdx >= 0) {
            ships[selectedShipIdx].shape->setPosition((ntl::Vector2f)mouseMoved->position);
        }
    }
    else return false;

    return true;
}

// void ShipPlacementMenu::draw(ntl::Window &window, ntl::RenderStates states) const {
//     for (auto ship : ships) {
//         auto bounds = ship.shape->getBounds();
//         auto [x, y] = bounds.position - ntl::Vector2f{2, 2};
//         auto [w, h] = bounds.size + ntl::Vector2f{4, 4};
//         window.draw(ntl::Shape({
//             {{x, y}, ntl::Color::Red},
//             {{x + w, y}, ntl::Color::Red},
//             {{x + w, y + h}, ntl::Color::Red},
//             {{x, y + h}, ntl::Color::Red},
//         }, ntl::PrimitiveType::TriangleFan));
//     }

//     App::draw(window, states);
// }

// void ShipPlacementMenu::update(ntl::Time delta) {
// }