#include "ShipPlacementMenu.hpp"
#include "Core.hpp"
#include "TextureManager.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>

ShipPlacementMenu::ShipPlacementMenu(ntl::Window &window) :
    App(window, ntl::Color(0x7aa1cdff)),
    grid({
        {playerGraphics[0], Core::LAYER_GRID, true},
        {playerGraphics[1], Core::LAYER_GRID, true},
    }),
    gameApp(window)
{
    divider = graphics.add<ntl::RectangleShape>(
        0,
        ntl::Vector2f(3.f, (float)window.getSize().y),
        ntl::Color(0x152b44ff)
    );
    divider->setPosition({float(window.getSize().x / 2), 0.f});

    nextButton = graphics.add<ntl::Sprite>(
        0, TextureManager::getInstance()[TextureManager::ID::NextButton]
    );
    nextButton->setPosition({
        window.getSize().x * 0.75f - nextButton->getLocalBounds().size.x / 2.f,
        window.getSize().y - nextButton->getLocalBounds().size.y - Core::getCellsize()
    });
    graphics.setVisibility(nextButton, false);

    for (int playerNumber = 0; playerNumber < 2; ++playerNumber) {
        std::array<int, 4> rangCount{};

        for (auto &ship : grid[playerNumber].ships) {
            ship.rotate(false);
            ship.drawable->setPosition(ntl::Vector2f{
                float(window.getSize().x / 2) + float((rangCount[ship.rang() - 1]++) * (ship.rang() + 1) + 1) * Core::getCellsize(),
                float(ship.rang() * 2 - 1) * Core::getCellsize(),
            } + ship.drawable->getOrigin());
            ship.menuPosition = ship.drawable->getPosition();
        }
    }
}

bool ShipPlacementMenu::handle(const ntl::Event &event) {
    auto &ships = this->grid[currentPlayerIdx].ships;

    if (const auto* keyPressed = event.getIf<ntl::Event::KeyPressed>()) {
        if (keyPressed->code == GLFW_KEY_R && selectedShipIdx >= 0) {
            ships[selectedShipIdx].rotate();
        } else if (keyPressed->code == GLFW_KEY_G) {
            autoPlaceShips();
        }
    } else if (const auto* mousePressed = event.getIf<ntl::Event::MouseButtonPressed>()) {
        if (mousePressed->button == GLFW_MOUSE_BUTTON_LEFT) {
            if (auto touched = graphics.getTouched((ntl::Vector2f)mousePressed->position)) {
                if (nextButton == touched->first) {
                    if (currentPlayerIdx) {
                        for (int i = 0; i < 2; ++i) {
                            for (int j = 0; j < grid[i].ships.size(); ++j) {
                                auto &newShip = gameApp.grid[i].ships[j];
                                const auto gridPosition = gameApp.grid[i].gridDrawable->getPosition(window.getSize());

                                newShip.cellPosition = grid[i].ships[j].cellPosition;
                                newShip.drawable->setPosition(
                                    gridPosition + (ntl::Vector2f)newShip.cellPosition * Core::getCellsize() + newShip.drawable->getOrigin()
                                );
                                newShip.rotate(grid[i].ships[j].vertical);
                            }
                        }

                        Core::setApp(&gameApp);
                    } else {
                        currentPlayerIdx = 1;
                        graphics.setVisibility(nextButton, false);
                    }
                }
            } else if (auto touched = playerGraphics[currentPlayerIdx].getTouched((ntl::Vector2f)mousePressed->position)) {
                selectedShipIdx = -1;

                for (int i = 0; i < ships.size(); ++i) {
                    if (ships[i].drawable == touched->first) selectedShipIdx = i;
                }

                if (selectedShipIdx >= 0) ships[selectedShipIdx].setLayer(Core::LAYER_SHIP_ACTIVE);
            }
        }
    } else if (const auto* mouseReleased = event.getIf<ntl::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == GLFW_MOUSE_BUTTON_LEFT && selectedShipIdx >= 0) {
            auto &selectedShip = ships[selectedShipIdx];
            auto gridPos = grid[currentPlayerIdx].gridDrawable->getPosition(window.getSize());
            auto cellPos = (selectedShip.drawable->getPosition() - gridPos) / Core::getCellsize();

            selectedShip.cellPosition = (ntl::Vector2i)cellPos;
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

                selectedShip.drawable->setPosition(
                    gridPos + cellPosTrunc * Core::getCellsize() + selectedShip.drawable->getOrigin().componentWiseMul(selectedShip.drawable->getScale())
                );
            } else {
                selectedShip.cellPosition = {-1, -1};
                selectedShip.drawable->setPosition(selectedShip.menuPosition);
                selectedShip.rotate(false);
            }

            selectedShip.setLayer(Core::LAYER_SHIP);
            selectedShipIdx = -1;

            bool isAllShipsPlaced = std::all_of(ships.begin(), ships.end(), [&](const Ship &ship) {
                return ship.cellPosition.x >= 0 && ship.cellPosition.y >= 0;
            });
            graphics.setVisibility(nextButton, isAllShipsPlaced);
        }
    } else if (const auto* mouseMoved = event.getIf<ntl::Event::MouseMoved>()) {
        if (selectedShipIdx >= 0) {
            ships[selectedShipIdx].drawable->setPosition((ntl::Vector2f)mouseMoved->position);
        }
    }
    else return false;

    return true;
}

void ShipPlacementMenu::draw(ntl::Window &window, ntl::RenderStates states) const {
    window.draw(playerGraphics[currentPlayerIdx], states);
    App::draw(window, states);
}

void ShipPlacementMenu::autoPlaceShips() {
    auto& currentGrid = grid[currentPlayerIdx];
    auto& ships = currentGrid.ships;
    const auto gridPos = currentGrid.gridDrawable->getPosition(window.getSize());

    auto isValidPosition = [&](const Ship& ship, int x, int y, bool isVertical) {
        if (isVertical) {
            if (y + ship.rang() > 10) return false;
        } else {
            if (x + ship.rang() > 10) return false;
        }

        ntl::IntRect bounds(
            {x, y},
            isVertical ? ntl::Vector2i(1, ship.rang()) : ntl::Vector2i(ship.rang(), 1)
        );

        for (const auto& other : ships) {
            if (&other == &ship || other.cellPosition.x < 0) continue;

            if (other.getBounds(true).isIntersect(bounds)) {
                return false;
            }
        }

        return true;
    };

    std::random_device device;
    std::default_random_engine engine(device());

    bool isErrorToPlace = false;
    do {
        for (auto& ship : ships) {
            ship.cellPosition = {-1, -1};
            ship.rotate(false);
            ship.drawable->setPosition(ship.menuPosition);
        }

        for (auto& ship : ships) {
            struct Valid {
                ntl::Vector2i position;
                int vertical{};
            };

            std::vector<Valid> validPlaces;

            for (int x = 0; x < 10; ++x) {
                for (int y = 0; y < 10; ++y) {
                    for (int vertical = 0; vertical < 2; ++vertical) {
                        if (isValidPosition(ship, x, y, vertical)) {
                            validPlaces.push_back({{x, y}, vertical});
                        }
                    }
                }
            }

            if (!validPlaces.empty()) {
                std::uniform_int_distribution<int> uniform(0, validPlaces.size() - 1);
                auto &validPlace = validPlaces[uniform(engine)];
                ship.cellPosition = validPlace.position;
                ship.rotate(validPlace.vertical);
                ship.drawable->setPosition(
                    gridPos +
                    (ntl::Vector2f)ship.cellPosition * Core::getCellsize() +
                    ship.drawable->getOrigin().componentWiseMul(ship.drawable->getScale())
                );
            } else {
                isErrorToPlace = true;
                break;
            }
        }
    } while (isErrorToPlace);

    graphics.setVisibility(nextButton, true);
}