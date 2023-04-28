#include "../include/canvas.hxx"
#include <iostream>

int main() {
    graphics::Canvas canvas{ 1920, 1080 };
    canvas.clear(graphics::green);

    canvas.save_image("canvas.ppm");
}
