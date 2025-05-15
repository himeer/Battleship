#include <iostream>

#include "NTL/Graphics.hpp"

#include "ShipPlacementMenu.hpp"
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