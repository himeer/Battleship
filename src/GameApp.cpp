#include "GameApp.hpp"
#include "Core.hpp"
#include "TextureManager.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

GameApp::GameApp(ntl::Window &window) :
    App(window, ntl::Color(0x7aa1cdff)),
    grid({
        {graphics, LAYER_GRID, true},
        {graphics, LAYER_GRID, false},
    })
{
    divider = graphics.add<ntl::RectangleShape>(
        0,
        ntl::Vector2f(3.f, (float)window.getSize().y),
        ntl::Color(0x152b44ff)
    );
    divider->setPosition({float(window.getSize().x / 2), 0.f});

    yourMoveText = graphics.add<ntl::Sprite>(
        0, TextureManager::getInstance()[TextureManager::ID::YourMove]
    );
}

bool GameApp::handle(const ntl::Event &event) {
    if (const auto* mousePressed = event.getIf<ntl::Event::MouseButtonPressed>()) {
        if (mousePressed->button == GLFW_MOUSE_BUTTON_LEFT && !isWin) {
            auto &currentGrid = grid[currentPlayerIdx];
            if (currentGrid.touch(window.getSize(), (ntl::Vector2f)mousePressed->position) == 1) {
                if (std::all_of(currentGrid.ships.begin(), currentGrid.ships.end(), [](const Ship &ship) {
                    return ship.getState() == Ship::State::Defeated;
                })) {
                    yourMoveText->setTexture(&TextureManager::getInstance()[TextureManager::ID::WinText]);
                    isWin = true;
                } else {
                    currentPlayerIdx = !currentPlayerIdx;
                }
            }
        }
    }
    else return false;

    return true;
}

void GameApp::draw(ntl::Window &window, ntl::RenderStates states) const {
    yourMoveText->setPosition({
        window.getSize().x * 0.25f * (currentPlayerIdx ? 3 : 1) - yourMoveText->getLocalBounds().size.x / 2.f,
        0
    });

    App::draw(window, states);
}