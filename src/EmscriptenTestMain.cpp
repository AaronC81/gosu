#include <Gosu/Platform.hpp>
#include <Gosu/Window.hpp>
#include <Gosu/Graphics.hpp>
#include <Gosu/Math.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/Input.hpp>

#include <iostream>
#include <emscripten.h>

#ifdef GOSU_IS_EMSCRIPTEN

class GameWindow : public Gosu::Window
{
public:
    GameWindow() : Window(500, 500), i("shark.png"), x(20.0), y(20.0) {}

    void draw() override
    {
        i.draw(x, y);
    }

    void update() override {
        if (Gosu::Input::down(Gosu::Button::KB_RIGHT)) {
            x += 1;
        } else if (Gosu::Input::down(Gosu::Button::KB_LEFT)) {
            x -= 1;
        }
        
        if (Gosu::Input::down(Gosu::Button::KB_UP)) {
            y -= 1;
        } else if (Gosu::Input::down(Gosu::Button::KB_DOWN)) {
            y += 1;
        }
    }

private:
    Gosu::Image i;

    double x;
    double y;
};

GameWindow window;

void tick() {
    window.tick();
}

int main() {
    emscripten_set_main_loop(tick, 60, 0);
}

#endif