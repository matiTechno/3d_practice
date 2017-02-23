#ifndef CAMERA_3D_HPP
#define CAMERA_3D_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera_3D
{
public:
    Camera_3D();

    glm::mat4 get_view_matrix() const;

private:
    glm::vec3 position;
    glm::vec3 up_vec;
    glm::vec3 direction;
};

#endif // CAMERA_3D_HPP
