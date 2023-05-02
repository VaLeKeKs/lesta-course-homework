//
// Created by valentin on 02.05.23.
//
#include <SDL3/SDL.h>
#include <chrono>

#include "../include/gfx.hxx"
#include "../include/triangle_interpolated.hxx"

using namespace std::literals;

namespace graphics {
    class GfxProgram : public graphics::IGfx {
    private:
        Color m_color{};
        double m_scale{1};
        double m_mouseX{};
        double m_mouseY{};

    public:
        void setUniforms(const graphics::Uniform &uniform) override {
            m_scale = uniform.p1;
            m_mouseX = uniform.p2;
            m_mouseY = uniform.p3;

            m_color.r = uniform.p4;
            m_color.g = uniform.p5;
            m_color.b = uniform.p6;

        };

        Color fragmentShader(const graphics::Vertex &vertex) override {
            Color color{vertex.extractColor()};
            color.r+=m_color.r;
            color.g+=m_color.g;
            color.b+=m_color.b;

            return color;
        };

        Vertex vertexShader(const graphics::Vertex &vertex) override {
            Vertex vertex1{vertex};
            vertex1.x *= m_scale;
            vertex1.y *= m_scale;
            return vertex1;
        };
    };

} // namespace graphics

int main() {
    constexpr int width{640};
    constexpr int height{480};

    SDL_Init(SDL_INIT_EVERYTHING);
    auto window{SDL_CreateWindow("render", width, height, SDL_WINDOW_OPENGL)};
    auto renderer{SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED)};

    graphics::Canvas canvas{width, height};
    graphics::GfxProgram gfx;
    graphics::TriangleInterpolateRender render{canvas, width, height, gfx};

    std::vector<graphics::Vertex> verticesBuffer{};
    std::vector<std::uint16_t> indicesBuffer{};

    void *pixels{canvas.get_pixels().data()};
    constexpr int pitch{sizeof(graphics::Color) * width};

    graphics::Uniform uniform{};

    double &scale = uniform.p1 = 1;
    double &mouseX = uniform.p2;
    double &mouseY = uniform.p3;

    double &r = uniform.p4;
    double &g = uniform.p5;
    double &b = uniform.p6;


    constexpr double scaleStep{0.05};

    bool isExit{false};

    auto time = std::chrono::steady_clock::now();

    while (!isExit) {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isExit = true;
                break;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    verticesBuffer.push_back({event.motion.x, event.motion.y, 0, 255, 0});
                    verticesBuffer.push_back({event.motion.x + 25, event.motion.y - 25, 255, 0, 0});
                    verticesBuffer.push_back({event.motion.x - 25, event.motion.y - 25, 0, 0, 255});
                    indicesBuffer.push_back(verticesBuffer.size() - 1);
                    indicesBuffer.push_back(verticesBuffer.size() - 2);
                    indicesBuffer.push_back(verticesBuffer.size() - 3);
                }
            }
        }

        canvas.clear(graphics::black);

        auto now = std::chrono::steady_clock::now();

        if (now - time >= 2s) {
            r+=6;
            g+=6;
            b+=6;
            if(r>255 || g>255 || b > 255){
                r=0;
                g=0;
                b=0;
            }
        }

        gfx.setUniforms(uniform);

        render.drawTriangles(verticesBuffer, indicesBuffer);

        auto bitmapSurface =
                SDL_CreateSurfaceFrom(pixels, width, height, pitch, SDL_PIXELFORMAT_RGB24);

        auto texture = SDL_CreateTextureFromSurface(renderer, bitmapSurface);

        SDL_DestroySurface(bitmapSurface);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}