#ifndef TEXT_HPP
#define TEXT_HPP

#include "font_loader.hpp"
#include <string>
class Renderer_2D;
#include "render_obj_base.hpp"

// no kerning support for now
class Text: public Render_obj_base
{
public:
    friend class Renderer_2D;

    Text(Font* font):
        scale(1.f),
        font(font)
    {}

    void set_pixel_size(unsigned size)
    {
        scale = static_cast<float>(size) / static_cast<float>(font->pixel_size);
    }

    glm::vec2 getSize() const;

    std::string text;
    float scale;

private:
    const Font* font;
};

#endif // TEXT_HPP
