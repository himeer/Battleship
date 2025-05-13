#ifndef APP_HPP
#define APP_HPP

#include "NTL/Graphics.hpp"
#include "GraphicManager.hpp"

class App : public ntl::Drawable {
public:
    App(const App&) = delete;
    App& operator=(const App&) = delete;
    // TODO: move constructor and move assignment

    virtual bool handle(const ntl::Event &event) = 0;

    // virtual void update(ntl::Time delta) = 0;

public:
    ntl::Color backgroundColor;

protected:
    GraphicManager graphics;
    ntl::RenderWindow &window;

    App(ntl::RenderWindow &window, ntl::Color backgroundColor = ntl::Color::Black) :
        window(window), backgroundColor(backgroundColor)
    {}

    void draw(ntl::Window &target) const override {
        target.draw(graphics);
    }
};

#endif // APP_HPP