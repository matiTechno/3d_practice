#ifndef CAMERA_3D_HPP
#define CAMERA_3D_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

enum class Cam_move_dir
{
    forward,
    backward,
    left,
    right
};

enum class Cam_rotation_dir
{
    up,
    down,
    left,
    right
};

// this camera is only for testing purposes
class Camera_3D
{
public:
    Camera_3D();

    glm::mat4 get_view_matrix() const;

    void move(Cam_move_dir mv_dir, float dt);

    void rotate(Cam_rotation_dir rt_dir, float dt);

private:
    glm::vec3 cam_pos;
    glm::vec3 cam_up;
    glm::vec3 cam_forward;

    float mv_speed;
    float rotation_speed;
};

#endif // CAMERA_3D_HPP
