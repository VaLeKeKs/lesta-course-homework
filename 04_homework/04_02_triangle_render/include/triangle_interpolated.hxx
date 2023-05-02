//
// Created by valentin on 02.05.23.
//

#ifndef INC_04_02_TRIANGLE_RENDER_TRIANGLE_INTERPOLATED_HXX
#define INC_04_02_TRIANGLE_RENDER_TRIANGLE_INTERPOLATED_HXX
#include <vector>
#include <array>
#include <functional>

#include "canvas.hxx"
#include "triangle_indexed.hxx"
#include "gfx.hxx"

namespace graphics {

    struct Vertex {
        double x{};
        double y{};

        double r{};
        double g{};
        double b{};

        Position extractPosition() const {
            return {static_cast<std::size_t>(x), static_cast<std::size_t>(y)};
        }

        Color extractColor() const {
            return {static_cast<std::uint8_t>(r),
                    static_cast<std::uint8_t>(g),
                    static_cast<std::uint8_t>(b)};
        }
    };

    static double interpolate(const double start, const double end, const double t) {
        if (t < 0 || t > 1) throw std::runtime_error{"Error : interpolate : t < 0 || t > 1"};
        return start + (end - start) * t;
    }

    static Vertex interpolate(const Vertex &start, const Vertex &end, const double t) {
        return {interpolate(start.x, end.x, t),
                interpolate(start.y, end.y, t),
                interpolate(start.r, end.r, t),
                interpolate(start.g, end.g, t),
                interpolate(start.b, end.b, t)};
    }

    class TriangleInterpolateRender : public TriangleIndexedRender {
    private:
        IGfx &m_gfx;

    public:
        TriangleInterpolateRender(Canvas &canvas, std::size_t width, std::size_t height, IGfx &gfx)
                : TriangleIndexedRender(canvas, width, height), m_gfx{gfx} {}

        void drawTriangles(const std::vector<Vertex> &vertices,
                           const std::vector<std::uint16_t> &indices) {
            if (indices.size() % 3 != 0)
                throw std::runtime_error{"Error : drawTriangles : indices.size() % 3 != 0"};

            for (std::size_t i{}; i < indices.size() / 3; ++i) {
                auto index0{indices.at(i * 3 + 0)};
                auto index1{indices.at(i * 3 + 1)};
                auto index2{indices.at(i * 3 + 2)};

                auto v0{vertices.at(index0)};
                auto v1{vertices.at(index1)};
                auto v2{vertices.at(index2)};

                v0 = m_gfx.vertexShader(v0);
                v1 = m_gfx.vertexShader(v1);
                v2 = m_gfx.vertexShader(v2);

                auto interpolatedTriangle{rasterizeTriangle(v0, v1, v2)};

                std::ranges::for_each(interpolatedTriangle, [&](const Vertex &vertex) {
                    auto pos = vertex.extractPosition();
                    canvas.set_pixel(pos, m_gfx.fragmentShader(vertex));
                });
            }
        }

    private:
        static std::vector<Vertex> rasterizeOneHorizontalLine(const Vertex &left,
                                                              const Vertex &right) {
            std::vector<Vertex> result{};
            std::size_t lineSize{static_cast<std::size_t>(std::abs(right.x - left.x))};
            if (lineSize > 0) {
                lineSize += 1;
                for (std::size_t i{}; i <= lineSize; ++i) {
                    double t{static_cast<double>(i) / (lineSize)};
                    auto pixel{interpolate(left, right, t)};
                    result.push_back(pixel);
                }
            } else
                result.push_back(left);

            return result;
        }

        static std::vector<Vertex>
        rasterizeOneHorizontalTriangle(const Vertex &left, const Vertex &right, const Vertex &single) {
            std::vector<Vertex> result{};

            std::size_t height{static_cast<std::size_t>(std::abs(left.y - single.y))};
            if (height > 0) {
                height += 1;
                for (std::size_t i{}; i <= height; ++i) {
                    double t{static_cast<double>(i) / (height)};
                    auto leftVertex{interpolate(left, single, t)};
                    auto rightVertex{interpolate(right, single, t)};

                    auto line{rasterizeOneHorizontalLine(leftVertex, rightVertex)};
                    std::move(line.begin(), line.end(), std::back_inserter(result));
                }
            } else
                result = rasterizeOneHorizontalLine(left, right);

            return result;
        }

        static std::vector<Vertex>
        rasterizeTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2) {
            std::vector<Vertex> result{};
            std::array<std::reference_wrapper<const Vertex>, 3> vertices{v0, v1, v2};
            std::ranges::sort(vertices, [](const Vertex &v1, const Vertex &v2) { return v1.y < v2.y; });

            auto top{vertices.at(0).get()};
            auto middle{vertices.at(1).get()};
            auto bottom{vertices.at(2).get()};

            double tSecondMiddle{std::abs(middle.y - top.y) / std::abs(top.y - bottom.y)};
            Vertex secondMiddle{interpolate(top, bottom, tSecondMiddle)};

            auto topTriangle = rasterizeOneHorizontalTriangle(middle, secondMiddle, top);
            std::move(topTriangle.begin(), topTriangle.end(), std::back_inserter(result));

            auto bottomTriangle = rasterizeOneHorizontalTriangle(middle, secondMiddle, bottom);
            std::move(bottomTriangle.begin(), bottomTriangle.end(), std::back_inserter(result));

            return result;
        };
    };

} // namespace graphics
#endif //INC_04_02_TRIANGLE_RENDER_TRIANGLE_INTERPOLATED_HXX
