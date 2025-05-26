#ifndef GAME_APP_HPP
#define GAME_APP_HPP 

#include <vector>
#include <array>
#include "App.hpp"
#include "Ship.hpp"
#include "GameGrid.hpp"

class ShipPlacementMenu;

class GameApp : public App {
public:
    GameApp(ntl::Window &window);

    virtual bool handle(const ntl::Event &event) override;

    virtual void draw(ntl::Window &window, ntl::RenderStates states) const override;

protected:
    friend ShipPlacementMenu;

    std::array<GameGrid, 2> grid;
    ntl::RectangleShape *divider{};
    ntl::Sprite *yourMoveText{};
    int currentPlayerIdx = 0;
    bool isWin = false;
};

#endif // GAME_APP_HPP