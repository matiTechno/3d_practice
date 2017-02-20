#ifndef RENDER_OBJ_BASE_HPP
#define RENDER_OBJ_BASE_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Render_obj_base
{
public:
    Render_obj_base():
        color(255.f, 255.f, 255.f, 1.f),
        rotation(0.f),
        bloom(false)
    {}

    glm::vec2 position;
    glm::vec4 color;
    // rotation_point is relative to sprite top-left corner
    glm::vec2 rotation_point;
    // radians
    float rotation;
    bool bloom;
};

#endif // RENDER_OBJ_BASE_HPP
