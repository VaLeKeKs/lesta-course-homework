//
// Created by valentin on 02.05.23.
//

#ifndef INC_04_02_TRIANGLE_RENDER_GFX_HXX
#define INC_04_02_TRIANGLE_RENDER_GFX_HXX
namespace graphics {

    struct Vertex;
    struct Color;

    struct Uniform {
        double p1{};
        double p2{};
        double p3{};
        double p4{};
        double p5{};
        double p6{};
        double p7{};
    };

    class IGfx {
    public:
        virtual ~IGfx() = default;

        virtual void setUniforms(const Uniform &uniform) = 0;

        virtual Vertex vertexShader(const Vertex &vertex) = 0;

        virtual Color fragmentShader(const Vertex &vertex) = 0;
    };

} // namespace graphics
#endif //INC_04_02_TRIANGLE_RENDER_GFX_HXX
