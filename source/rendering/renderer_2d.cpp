#include "renderer_2d.hpp"
#include <assert.h>
#include "sprite.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "text.hpp"

bool Renderer_2D::isCurrent = false;

Renderer_2D::Renderer_2D()
{
    assert(!isCurrent);
    isCurrent = true;

    shader = std::make_unique<Shader>("shaders/shader_2d.vert", "shaders/shader_2d.frag", "", true,
                                      "renderer_2D");

    sampler.set_parameter_i(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    sampler.set_parameter_i(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    sampler.set_parameter_i(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    sampler.set_parameter_i(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLfloat vertices[] =
    {
        // pos....texCoord
        0.f, 0.f, 0.f, 0.f,
        1.f, 0.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 0.f, 1.f,
        0.f, 0.f, 0.f, 0.f
    };

    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Renderer_2D::render(const Sprite& sprite) const
{
    vao.bind();
    shader->bind();
    sampler.bind();

    if(sprite.texture)
    {
        sprite.texture->bind();
        glUniform1i(shader->getUniLocation("type"), 0);

        auto& coords = sprite.texCoords;
        auto texSize = sprite.texture->getSize();

        glm::vec2 texSize_gl(static_cast<float>(coords.z) / static_cast<float>(texSize.x),
                             static_cast<float>(coords.w) / static_cast<float>(texSize.y));
        glm::vec2 texShift(static_cast<float>(coords.x) / static_cast<float>(texSize.x),
                           static_cast<float>(coords.y) / static_cast<float>(texSize.y));

        glUniform2f(shader->getUniLocation("texSize"), texSize_gl.x, texSize_gl.y);
        glUniform2f(shader->getUniLocation("texShift"), texShift.x, texShift.y);

    }
    else
        glUniform1i(shader->getUniLocation("type"), 1);

    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(sprite.position, 0.f));
    if(sprite.rotation != 0.f)
    {
        model = glm::translate(model, glm::vec3(sprite.rotation_point, 0.f));
        model = glm::rotate(model, sprite.rotation, glm::vec3(0.f, 0.f, -1.f));
        model = glm::translate(model, glm::vec3(-sprite.rotation_point, 0.f));
    }
    model = glm::scale(model, glm::vec3(sprite.size, 1.f));

    glUniformMatrix4fv(shader->getUniLocation("model"), 1, GL_FALSE, &model[0][0]);

    auto& col = sprite.color;
    glUniform4f(shader->getUniLocation("spriteColor"), col.x/255.f, col.y/255.f, col.z/255.f, col.w);

    if(sprite.bloom)
        glUniform1f(shader->getUniLocation("isBloom"), true);
    else
        glUniform1f(shader->getUniLocation("isBloom"), false);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer_2D::render(const Text& text) const
{
    vao.bind();
    shader->bind();
    sampler.bind();
    text.font->atlas.bind();

    glUniform1i(shader->getUniLocation("type"), 2);

    if(text.bloom)
        glUniform1f(shader->getUniLocation("isBloom"), true);
    else
        glUniform1f(shader->getUniLocation("isBloom"), false);

    auto& col = text.color;
    glUniform4f(shader->getUniLocation("spriteColor"), col.x/255.f, col.y/255.f, col.z/255.f, col.w);

    glm::vec2 pen_pos = text.position;

    bool first_char_in_line = true;

    for(auto c: text.text)
    {
        if(c == '\n')
        {
            pen_pos.x = text.position.x;
            pen_pos.y += static_cast<float>(text.font->line_space) * text.scale;
            first_char_in_line = true;
            continue;
        }

        assert(c >= 32 && c < 127);

        auto& this_char = text.font->chars.at(c);

        // texture coordinates
        {
            auto& coords = this_char.texCoords;
            auto texSize = text.font->atlas.getSize();

            glm::vec2 texSize_gl(static_cast<float>(coords.z) / static_cast<float>(texSize.x),
                                 static_cast<float>(coords.w) / static_cast<float>(texSize.y));
            glm::vec2 texShift(static_cast<float>(coords.x) / static_cast<float>(texSize.x),
                               static_cast<float>(coords.y) / static_cast<float>(texSize.y));

            glUniform2f(shader->getUniLocation("texSize"), texSize_gl.x, texSize_gl.y);
            glUniform2f(shader->getUniLocation("texShift"), texShift.x, texShift.y);
        }
        // model matrix
        {
            glm::vec2 glyph_size(this_char.texCoords.z * text.scale, this_char.texCoords.w * text.scale);
            glm::vec2 glyph_pos;

            if(first_char_in_line)
            {
                glyph_pos = glm::vec2(pen_pos.x,
                                      pen_pos.y + static_cast<float>(text.font->max_bearing_y - this_char.bearing.y)
                                      * text.scale);

                first_char_in_line = false;
            }
            else
            {
                glyph_pos = glm::vec2(pen_pos.x + static_cast<float>(this_char.bearing.x) * text.scale,
                                      pen_pos.y + static_cast<float>(text.font->max_bearing_y - this_char.bearing.y)
                                      * text.scale);
            }

            glm::mat4 model(1.f);
            model = glm::translate(model, glm::vec3(glyph_pos, 0.f));
            if(text.rotation != 0.f)
            {
                model = glm::translate(model, glm::vec3(text.rotation_point - (glyph_pos - text.position), 0.f));
                model = glm::rotate(model, text.rotation, glm::vec3(0.f, 0.f, -1.f));
                model = glm::translate(model, glm::vec3(-text.rotation_point + (glyph_pos - text.position), 0.f));
            }
            model = glm::scale(model, glm::vec3(glyph_size, 1.f));

            glUniformMatrix4fv(shader->getUniLocation("model"), 1, GL_FALSE, &model[0][0]);
        }

        pen_pos.x += static_cast<float>(this_char.advance) * text.scale;
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Renderer_2D::load_projection(const glm::mat4& matrix)
{
    shader->bind();
    glUniformMatrix4fv(shader->getUniLocation("projection"), 1, GL_FALSE, &matrix[0][0]);
    shader->bind();
    glUniformMatrix4fv(shader->getUniLocation("projection"), 1, GL_FALSE, &matrix[0][0]);
}
