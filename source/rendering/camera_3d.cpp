#include "camera_3d.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera_3D::Camera_3D():
    cam_pos(0.f, 0.f, 0.f),
    cam_up(0.f, 1.f, 0.f),
    cam_forward(0.f, 0.f, -1.f),
    mv_speed(2.f),
    rotation_speed(glm::pi<float>() * 2.f / 4.f)
{}

glm::mat4 Camera_3D::get_view_matrix() const
{
    return glm::lookAt(cam_pos, cam_pos + cam_forward, cam_up);
}

void Camera_3D::move(Cam_move_dir mv_dir, float dt)
{
    if(mv_dir == Cam_move_dir::forward)
        cam_pos += cam_forward * mv_speed * dt;
    else if(mv_dir == Cam_move_dir::backward)
        cam_pos -= cam_forward * mv_speed * dt;
    else if(mv_dir == Cam_move_dir::left)
        cam_pos += glm::cross(cam_up, cam_forward) * mv_speed * dt;
    else if(mv_dir == Cam_move_dir::right)
        cam_pos -= glm::cross(cam_up, cam_forward) * mv_speed * dt;
}

void Camera_3D::rotate(Cam_rotation_dir rt_dir, float dt)
{
    if(rt_dir == Cam_rotation_dir::up)
    {
        glm::vec3 cam_right = -glm::cross(cam_up, cam_forward);
        cam_forward = glm::rotate(cam_forward, rotation_speed * dt, cam_right);
        cam_up = glm::cross(cam_right, cam_forward);
    }
    else if(rt_dir == Cam_rotation_dir::down)
    {
        glm::vec3 cam_right = -glm::cross(cam_up, cam_forward);
        cam_forward = glm::rotate(cam_forward, -rotation_speed * dt, cam_right);
        cam_up = glm::cross(cam_right, cam_forward);
    }
    else if(rt_dir == Cam_rotation_dir::left)
    {
        glm::vec3 y_axis(0.f, 1.f, 0.f);
        cam_forward = glm::rotate(cam_forward, rotation_speed * dt, y_axis);
        cam_up = glm::rotate(cam_up, rotation_speed * dt, y_axis);
    }
    else if(rt_dir == Cam_rotation_dir::right)
    {
        glm::vec3 y_axis(0.f, 1.f, 0.f);
        cam_forward = glm::rotate(cam_forward, -rotation_speed * dt, y_axis);
        cam_up = glm::rotate(cam_up,-rotation_speed * dt, y_axis);
    }
}
