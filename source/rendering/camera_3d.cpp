#include "camera_3d.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera_3D::Camera_3D():
    position(0.f, 0.f, 0.f),
    up_vec(0.f, 1.f, 0.f),
    direction(0.f, 0.f, -1.f)
{}

glm::mat4 Camera_3D::get_view_matrix() const
{
    return glm::lookAt(position, position + direction, up_vec);
}
