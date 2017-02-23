#ifndef RENDERER_3D_HPP
#define RENDERER_3D_HPP

#include "../opengl/shader.hpp"
#include <glm/mat4x4.hpp>
class Model_3D;

class Renderer_3D
{
public:
    Renderer_3D();
    Renderer_3D(const Renderer_3D&) = delete;

    void render(const Model_3D& model) const;

    void load_projection(const glm::mat4& matrix) const;
    void load_view_matrix(const glm::mat4& matrix);

private:
    static bool isCurrent;

    Shader shader;
    glm::mat4 view_matrix;
};

#endif // RENDERER_3D_HPP
