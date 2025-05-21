#ifndef SHIP_PLACEMENT_MENU_HPP
#define SHIP_PLACEMENT_MENU_HPP

#include <vector>
#include <array>
#include "App.hpp"
#include "Ship.hpp"
#include "GameGrid.hpp"
#include "GameApp.hpp"

class ShipPlacementMenu : public App {
public:
    ShipPlacementMenu(ntl::Window &window);

    virtual bool handle(const ntl::Event &event) override;

    // virtual void update(ntl::Time delta) override;

    virtual void draw(ntl::Window &window, ntl::RenderStates states) const override;

protected:
    std::array<GraphicManager, 2> playerGraphics;
    ntl::RectangleShape *divider = nullptr;
    ntl::Sprite *nextButton = nullptr;
    std::array<GameGrid, 2> grid;
    int selectedShipIdx = -1;
    int currentPlayerIdx = 0;

    GameApp gameApp;

    void testPlaceShips();
};

#endif // SHIP_PLACEMENT_MENU_HPP