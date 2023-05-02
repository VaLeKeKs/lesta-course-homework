//
// Created by valentin on 22.04.23.
//

#ifndef INC_04_00_CANVAS_CANVAS_HXX
#define INC_04_00_CANVAS_CANVAS_HXX

#include <algorithm>
#include <compare>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace graphics {

    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;

        auto operator<=>(const Color &color) const = default;
    };

    struct Position {
        std::size_t x = 0;
        std::size_t y = 0;

        static Position generate_random(std::size_t width, std::size_t height) {
            return {rand() % width, rand() % height};
        }

        auto operator<=>(const Position &position) const = default;
    };

    constexpr Color red{255, 0, 0};
    constexpr Color green{0, 255, 0};
    constexpr Color blue{0, 0, 255};
    constexpr Color black{0, 0, 0};

    static std::ofstream &operator<<(std::ofstream &out, const std::vector<Color> &pixels) {
        out.write(reinterpret_cast<const char *>(pixels.data()),
                  pixels.size() * sizeof(std::vector<Color>::value_type));
        return out;
    }

    static std::ifstream &operator>>(std::ifstream &in, std::vector<Color> &pixels) {
        in.read(reinterpret_cast<char *>(pixels.data()),
                pixels.size() * sizeof(std::vector<Color>::value_type));
        return in;
    }

    class Canvas {
    public:
        Canvas(std::size_t w, std::size_t h) : width{w}, height{h} {
            pixels.resize(width * height);
        }

        void save_image(const std::string &file_name) {
            std::ofstream out(file_name.data(), std::ios::binary);
            out.exceptions(std::ios::failbit);
            out << "P6\n" << width << ' ' << height << ' ' << '\n' << 255 << '\n';
            out << pixels;
        }

        void load_image(const std::string &file_name) {
            std::ifstream in{file_name.data(), std::ios::binary};
            in.exceptions(std::ios::failbit);
            std::string header;
            std::string color_format;
            char last_next_line = 0;
            in >> header >> width >> height >> color_format >> std::noskipws >> last_next_line;
            if (!iswspace(last_next_line)) { throw std::runtime_error("expected whitespace."); }

            pixels.resize(width * height);

            if (pixels.size() != width * height) { throw std::runtime_error("pixels size not match."); }

            in >> pixels;
        }

        void set_pixel(Position &position, Color col) {
            pixels.at(position.y * width + position.x) = col;
        }

        std::vector<Color>& get_pixels() { return pixels; }

        Color get_pixel(Position &position) { return pixels.at(position.y * width + position.x); }

        void clear(Color color) { std::ranges::fill(pixels, color); }

        auto begin() { return pixels.begin(); }

        auto end() { return pixels.end(); }

    private:
        std::size_t width = 0;
        std::size_t height = 0;
        std::vector<Color> pixels;
    };

    using pixels = std::vector<Position>;

    struct IRender {
        virtual void clear(Color) = 0;

        virtual void set_pixel(Position, Color) = 0;

        virtual pixels pixels_position(Position start, Position end) = 0;

        virtual ~IRender() = default;
    };

} // namespace graphics

#endif // INC_04_00_CANVAS_CANVAS_HXX
