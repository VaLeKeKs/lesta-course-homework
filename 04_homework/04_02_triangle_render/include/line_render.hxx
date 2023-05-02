//
// Created by valentin on 27.04.23.
//

#ifndef INC_04_01_LINE_RENDER_LINE_RENDER_HXX
#define INC_04_01_LINE_RENDER_LINE_RENDER_HXX

#include <algorithm>

#include "canvas.hxx"

namespace graphics {

    class LineRender : public IRender {

    public:
        pixels pixels_position(graphics::Position start, graphics::Position end) override {
            pixels result;
            int x0 = start.x;
            int x1 = end.x;
            int y0 = start.y;
            int y1 = end.y;

            int dx = abs(x1 - x0);
            int dy = abs(y1 - y0);

            int stepX = x0 < x1 ? 1 : -1;
            int stepY = y0 < y1 ? 1 : -1;

            int error = dx - dy;

            if (dx > dy) {
                while (x0 != x1) {
                    result.push_back(Position(x0, y0));
                    x0 += stepX;
                    error -= 2 * dy;
                    if (error < 0) {
                        y0 += stepY;
                        error += 2 * dx;
                    }
                }
            } else {
                while (y0 != y1) {
                    result.push_back(Position(x0, y0));
                    y0 += stepY;
                    error -= 2 * dx;
                    if (error < 0) {
                        x0 += stepX;
                        error += 2 * dy;
                    }
                }
            }

            result.push_back(Position(x0, y0));

            return result;
        }

        void drawLine(Position start, Position end, Color col) {
            pixels l = pixels_position(start, end);
            std::for_each(begin(l), std::end(l), [&](auto &position) { set_pixel(position, col); });
        }

        void clear(graphics::Color) override {}

        void set_pixel(Position pos, Color col) override {
            canvas.set_pixel(pos, col);
        };

        LineRender(Canvas &canvas, std::size_t width, std::size_t height)
                : canvas{canvas}, w{width}, h{height} {}

    protected:
        Canvas &canvas;
        std::size_t w{};
        std::size_t h{};
    };
}

#endif //INC_04_01_LINE_RENDER_LINE_RENDER_HXX
