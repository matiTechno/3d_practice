#ifndef POSTPROCESSOR_HPP
#define POSTPROCESSOR_HPP

#include "../opengl/buffers.hpp"
#include "../opengl/texture.hpp"
#include <glm/vec2.hpp>
#include "../opengl/shader.hpp"
#include <memory>

class Postprocessor
{
public:
    Postprocessor(int width, int height);
    Postprocessor(const Postprocessor&) = delete;

    void set_new_size(int width, int height);

    void begRender() const;
    void endRender(bool set_default_fb = true) const;
    void render(bool tone_mapping) const;

private:
    static bool isCurrent;

    glm::ivec2 fbSize;
    VAO vao;
    BO vbo;
    Sampler sampler;
    Shader shader_blur, shader_blend;
    std::unique_ptr<Texture> tex_base, tex_bright, tex_pp1, tex_pp2;
    FBO fb_beg, fb_pp1, fb_pp2;
    RBO rbo_depth;
};

#endif // POSTPROCESSOR_HPP
