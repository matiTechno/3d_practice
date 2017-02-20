#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "render_obj_base.hpp"
class Texture;

class Sprite: public Render_obj_base
{
public:
    Sprite():
        texture(nullptr)
    {}

    glm::vec2 size;
    glm::ivec4 texCoords;
    Texture* texture;
};

#endif // SPRITE_HPP
