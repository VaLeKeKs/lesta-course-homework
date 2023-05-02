//
// Created by valentin on 29.04.23.
//

#ifndef INC_04_02_TRIANGLE_RENDER_TRIANGLE_RENDER_HXX
#define INC_04_02_TRIANGLE_RENDER_TRIANGLE_RENDER_HXX

#include "canvas.hxx"
#include "line_render.hxx"

namespace graphics {

    class TriangleRender : public LineRender {
    public:
        TriangleRender(Canvas &canvas, std::size_t width, std::size_t height)
                : LineRender{canvas, width, height} {}

        virtual pixels pixelPositionsTriangle(Position v0, Position v1, Position v2) {
            pixels result{};
            for (const auto [start, end]:
                    {std::pair{v0, v1}, std::pair{v1, v2}, std::pair{v2, v0}}) {

                pixels line{LineRender::pixels_position(start, end)};
                result.insert(result.end(), line.begin(), line.end());
            }

            return result;
        }

        void drawTriangles(const std::vector<Position> &vertices, Color color) {
            if (vertices.size() % 3 != 0)
                throw std::runtime_error{"Error : drawTriangles : vertices.size() % 3 != 0"};

            for (std::size_t i{}; i < vertices.size() / 3; ++i) {
                auto v0{vertices.at(i * 3 + 0)};
                auto v1{vertices.at(i * 3 + 1)};
                auto v2{vertices.at(i * 3 + 2)};

                auto trianglePositions{pixelPositionsTriangle(v0, v1, v2)};
                std::ranges::for_each(trianglePositions, [&](Position position) {
                    canvas.set_pixel(position, color);
                });
            }
        }
    };

} // namespace graphics
#endif //INC_04_02_TRIANGLE_RENDER_TRIANGLE_RENDER_HXX
