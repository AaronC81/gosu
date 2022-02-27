#include <Gosu/Platform.hpp>
#include <Gosu/Window.hpp>
#include <Gosu/Graphics.hpp>
#include <Gosu/Math.hpp>

#ifdef GOSU_IS_EMSCRIPTEN

class GameWindow : public Gosu::Window
{
public:
    GameWindow() : Window(500, 500) {}

    void draw() override
    {
        Gosu::Graphics::draw_rect(
            Gosu::random(0.0, 350.0), Gosu::random(0.0, 350.0), 100.0, 100.0, Gosu::Color::WHITE, 0
        );
    }
};

#include <iostream>
#include <emscripten.h>

GameWindow window;

void tick() {
    window.tick();
}

int main() {
    std::cout << "Hello, world!\n" << std::endl;

    emscripten_set_main_loop(tick, 60, 0);
}

#endif