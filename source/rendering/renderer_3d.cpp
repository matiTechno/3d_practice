#include "renderer_3d.hpp"
#include <assert.h>
#include "model_3d.hpp"

bool Renderer_3D::isCurrent = false;

Renderer_3D::Renderer_3D():
    shader("shaders/shader_3d.vert", "shaders/shader_3d.frag", "", true, "Renderer_3d")
{
    assert(!isCurrent);
    isCurrent = true;
}

void Renderer_3D::render(const Model_3D& model) const
{
    shader.bind();
    model.mesh->vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.mesh->num_indices),
                   GL_UNSIGNED_INT, 0);
}
