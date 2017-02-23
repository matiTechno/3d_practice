#include "renderer_3d.hpp"
#include <assert.h>
#include "model_3d.hpp"
#include <glm/gtc/matrix_transform.hpp>

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

    glm::mat4 mat_model(1.f);
    mat_model = glm::translate(mat_model, model.position);
    mat_model = glm::rotate(mat_model, model.rotation.x, glm::vec3(-1.f, 0.f, 0.f));
    mat_model = glm::rotate(mat_model, model.rotation.y, glm::vec3(0.f, -1.f, 0.f));
    mat_model = glm::rotate(mat_model, model.rotation.z, glm::vec3(0.f, 0.f, 1.f));
    mat_model = glm::scale(mat_model, model.scale);

    glm::mat4 model_view_matrix = view_matrix * mat_model;

    glUniformMatrix4fv(shader.getUniLocation("model_view"), 1, GL_FALSE, &model_view_matrix[0][0]);

    model.mesh->vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.mesh->num_indices),
                   GL_UNSIGNED_INT, 0);
}

void Renderer_3D::load_projection(const glm::mat4& matrix) const
{
    shader.bind();
    glUniformMatrix4fv(shader.getUniLocation("projection"), 1, GL_FALSE, &matrix[0][0]);
}

void Renderer_3D::load_view_matrix(const glm::mat4& matrix)
{
    view_matrix = matrix;
}
