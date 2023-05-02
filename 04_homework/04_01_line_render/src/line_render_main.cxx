#include "../include/line_render.hxx"

int main() {
    const int32_t width = 1920;
    const int32_t height = 1080;

    graphics::Canvas canvas{width, height};
    canvas.clear(graphics::green);

    graphics::LineRender render{canvas, width, height};

    for (size_t i = 0; i < 100; ++i) {
        graphics::Position start{graphics::Position::generate_random(width, height)};
        graphics::Position end{graphics::Position::generate_random(width, height)};
        graphics::Color color{static_cast<uint8_t>(rand() % 256),
                              static_cast<uint8_t>(rand() % 256),
                              static_cast<uint8_t>(rand() % 256)};
        render.drawLine(start, end, color);
    }


    canvas.save_image("lines.ppm");
}
