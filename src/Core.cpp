#include <iostream>

#include <iostream>
#include "NTL/Graphics.hpp"

#include "Core.hpp"

int main() {
    ntl::Window window({1280, 720}, "Battleship");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            // Event tests here
            if (const auto *mousePressed = event->getIf<ntl::Event::MouseButtonPressed>()) {
                std::cout
                    << mousePressed->button << " "
                    << mousePressed->modes << " "
                    << "{" << mousePressed->position.x << mousePressed->position.y << "}"
                    << std::endl;
            }
        }

        window.clear(ntl::Color::White);

        // Draw tests here

        window.display();
    }

    return 0;
}