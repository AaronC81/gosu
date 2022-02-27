# Gosu for Emscripten

This is a **highly experimental** prototype of the [Gosu game engine](https://github.com/gosu/gosu)
running under Emscripten.

Only games written in C++ can run like this - no Ruby support yet.

There are definitely things that don't work, but the following features are tested working:

- Drawing rectangles
- Drawing images
- Keyboard input
- Playing samples

## How do I use it?

As prerequisites, you will need:

1. The Emscripten SDK, such that `emcc` is on your PATH. On macOS, I grabbed this with
   `brew install emscripten`.
2. Some way of hosting a local web server - Python will do!

The code of the example game is in `src/EmscriptenMain.cpp`, but you can change the structure as
much as you like as long as there's a definition of `int main()` somewhere.

To build everything, run `emscripten-build`! Then start a web server in `emscripten-output` and
navigate to `out.html`. (If you have Python, you can run `python -m http.server` in that directory,
and then navigate to `localhost:8000/out.html` in your browser.)

Any assets used by the game can be put into the `emscripten-assets` directory. The apparent
current working directory of the game when it runs in your browser will be inside this folder, so
you could load a file named `sprite.png` with `Gosu::Image("sprite.png")`.
