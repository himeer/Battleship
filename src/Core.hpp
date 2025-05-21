#ifndef CORE_HPP
#define CORE_HPP

namespace Core {
    void setApp(struct App *app);

    float getCellsize();

    static constexpr int LAYER_GRID = 0;
    static constexpr int LAYER_SHIP = 100;
    static constexpr int LAYER_SHIP_ACTIVE = LAYER_SHIP + 1;
}

#endif // CORE_HPP