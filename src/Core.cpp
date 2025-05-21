#include <iostream>

#include "NTL/Graphics.hpp"

#include "ShipPlacementMenu.hpp"
#include "TextureManager.hpp"
#include "Core.hpp"

namespace {
    App *_app = nullptr;
    float _cellsize = 0.f;
}

void Core::setApp(App *app) {
    _app = app;
}

float Core::getCellsize() {
    return _cellsize;
}


int main() {
    ntl::Window window({1280, 720}, "Battleship");

    _cellsize = window.getSize().x / 24;

    try {
        auto &textures = TextureManager::getInstance();

        textures.load(TextureManager::ID::NextButton, "assets/textures/nextButton.png");
        textures.load(TextureManager::ID::YourMove, "assets/textures/yourMove.png");
        textures.load(TextureManager::ID::WinText, "assets/textures/winText.png");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    ShipPlacementMenu defaultApp(window);
    Core::setApp(&defaultApp);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            _app->handle(*event);
        }

        window.clear(_app->backgroundColor);
        window.draw(*_app);
        window.display();
    }

    return 0;
}