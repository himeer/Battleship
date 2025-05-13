#ifndef SHIP_PLACEMENT_MENU_HPP
#define SHIP_PLACEMENT_MENU_HPP

#include <vector>
#include "App.hpp"
#include "Ship.hpp"
#include "GameGrid.hpp"

class ShipPlacementMenu : public App {
public:
    ShipPlacementMenu(ntl::Window &window);

    virtual bool handle(const ntl::Event &event) override;

    // virtual void update(ntl::Time delta) override;

protected:
    std::vector<Ship> ships;
    ntl::RectangleShape *divider = nullptr;
    int selectedShipIdx = -1;
    GameGrid grid;

    static constexpr int LAYER_GRID = 0;
    static constexpr int LAYER_SHIP = 100;
    static constexpr int LAYER_SHIP_ACTIVE = LAYER_SHIP + 1;
};

#endif // SHIP_PLACEMENT_MENU_HPP