#include "ShipPlacementMenu.hpp"
#include "Core.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

ShipPlacementMenu::ShipPlacementMenu(ntl::Window &window) :
    App(window, ntl::Color(0x7aa1cdff)),
    grid(graphics, LAYER_GRID)
{
    divider = graphics.add<ntl::RectangleShape>(0, ntl::Vector2f(3.f, (float)window.getSize().y));
    divider->setPosition({float(window.getSize().x / 2), 0.f});
    divider->setFillColor(ntl::Color(0x152b44ff));

    for (int i = 1; i <= 4; ++i) {
        for (int j = 0; j <= 4 - i; ++j) {
            ships.emplace_back(graphics, LAYER_SHIP, i);
            auto &ship = ships.back();
            ship.rotate(false);
            ship.sprite->setPosition({
                float(window.getSize().x / 2) + float(j * (i + 1) + 1) * Core::getCellsize() + ship.sprite->getOrigin().x,
                float(i * 2 - 1) * Core::getCellsize() + ship.sprite->getOrigin().y,
            });
            ship.menuPosition = ship.sprite->getPosition();
        }
    }
}

bool ShipPlacementMenu::handle(const ntl::Event &event) {
    if (const auto* keyPressed = event.getIf<ntl::Event::KeyPressed>()) {
        if (keyPressed->code == ntl::Keyboard::Key::R && selectedShipIdx >= 0) {
            ships[selectedShipIdx].rotate();
        }
    } else if (const auto* mousePressed = event.getIf<ntl::Event::MouseButtonPressed>()) {
        if (mousePressed->button == ntl::Mouse::Button::Left) {
            if (auto touched = graphics.getTouched((ntl::Vector2f)mousePressed->position)) {
                selectedShipIdx = -1;

                for (int i = 0; i < ships.size(); ++i) {
                    if (ships[i].sprite == touched->first) selectedShipIdx = i;
                }

                if (selectedShipIdx >= 0) ships[selectedShipIdx].setLayer(LAYER_SHIP_ACTIVE);
            }
        }
    } else if (const auto* mouseReleased = event.getIf<ntl::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == ntl::Mouse::Button::Left && selectedShipIdx >= 0) {
            auto &selectedShip = ships[selectedShipIdx];
            auto gridPos = grid.gridDrawable->getPosition(window.getSize());
            auto cellPos = (selectedShip.sprite->getPosition() - gridPos) / Core::getCellsize();

            selectedShip.areaPosition = (ntl::Vector2i)cellPos;
            auto shipBounds = selectedShip.getBounds(false);

            if (
                ((ntl::FloatRect)GameGrid::RECT).contains(cellPos)
                && GameGrid::RECT.contains(shipBounds.position)
                && GameGrid::RECT.contains(shipBounds.position + shipBounds.size - ntl::Vector2i(1, 1))
                && std::all_of(ships.begin(), ships.end(), [&](const Ship &ship) {
                    return &ship == &selectedShip || !ship.isIntersect(selectedShip);
                })
            ) {
                ntl::Vector2f cellPosTrunc(std::trunc(cellPos.x), std::trunc(cellPos.y));

                selectedShip.sprite->setPosition(
                    gridPos + cellPosTrunc * Core::getCellsize() + selectedShip.sprite->getOrigin().componentWiseMul(selectedShip.sprite->getScale())
                );
            } else {
                selectedShip.areaPosition = {-1, -1};
                selectedShip.sprite->setPosition(selectedShip.menuPosition);
            }

            selectedShip.setLayer(LAYER_SHIP);
            selectedShipIdx = -1;
        }
    } else if (const auto* mouseMoved = event.getIf<ntl::Event::MouseMoved>()) {
        if (selectedShipIdx >= 0) {
            ships[selectedShipIdx].sprite->setPosition((ntl::Vector2f)mouseMoved->position);
        }
    }
    else return false;

    return true;
}

// void ShipPlacementMenu::update(ntl::Time delta) {
// }