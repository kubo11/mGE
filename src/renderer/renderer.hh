#ifndef MGE_RENDERER_RENDERER_HH
#define MGE_RENDERER_RENDERER_HH

#include "mgepch.hh"

namespace mge {

enum class RendererType {
    NONE,
    LINES,
    TRIANGLES
};

class Renderer {
    public:
        virtual constexpr RenderType type = RendererType::NONE;

        Renderer();
        virtual ~Renderer();

        virtual void draw() const = 0;

    private:
};

class LineRenderer : public Renderer {
    virtual constexpr RenderType type = RendererType::LINES;
    public:
    LineRenderer();
    virtual ~LineRenderer() override;

    void set_vertices();
    virtual void draw() const override;

    private:
};

} // mge

#endif // MGE_RENDERER_RENDERER_HH