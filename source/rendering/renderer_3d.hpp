#ifndef RENDERER_3D_HPP
#define RENDERER_3D_HPP

#include "../opengl/shader.hpp"
class Model_3D;

class Renderer_3D
{
public:
    Renderer_3D();
    Renderer_3D(const Renderer_3D&) = delete;

    void render(const Model_3D& model) const;

private:
    static bool isCurrent;

    Shader shader;
};

#endif // RENDERER_3D_HPP
