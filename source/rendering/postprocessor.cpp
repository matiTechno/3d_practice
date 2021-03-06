#include "postprocessor.hpp"
#include <assert.h>

bool Postprocessor::isCurrent = false;

Postprocessor::Postprocessor(int width, int height):
    fbSize(0, 0),
    shader_blur("shaders/shader_fb.vert", "shaders/shader_fb_blur.frag", "", true, "fb_blur"),
    shader_blend("shaders/shader_fb.vert", "shaders/shader_fb_blend.frag", "", true, "fb_blend")
{
    assert(!isCurrent);
    isCurrent = true;

    sampler.set_parameter_i(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    sampler.set_parameter_i(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    sampler.set_parameter_i(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    sampler.set_parameter_i(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLfloat vertices[] =
    {
        // pos....texCoord
        -1.f, -1.f, 0.f, 0.f,
        1.f, -1.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 1.f, 1.f,
        -1.f, 1.f, 0.f, 1.f,
        -1.f, -1.f, 0.f, 0.f
    };

    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    fb_beg.bind();
    const GLenum attachments[] =
    {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
    };
    glDrawBuffers(2, attachments);

    rbo_depth.bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth.get_id());

    shader_blend.bind();
    glUniform1i(shader_blend.getUniLocation("base"), 0);
    glUniform1i(shader_blend.getUniLocation("blur"), 1);

    set_new_size(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Postprocessor::set_new_size(int width, int height)
{
    if(fbSize.x == width && fbSize.y == height)
        return;

    fbSize = glm::ivec2(width, height);

    fb_beg.bind();
    tex_base = std::make_unique<Texture>(GL_RGB16F, width, height);
    tex_bright = std::make_unique<Texture>(GL_RGB16F, width, height);
    rbo_depth.bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_base->get_id(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex_bright->get_id(), 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    fb_pp1.bind();
    tex_pp1 = std::make_unique<Texture>(GL_RGB16F, width / 2, height / 2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_pp1->get_id(), 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    fb_pp2.bind();
    tex_pp2 = std::make_unique<Texture>(GL_RGB16F, width / 2, height / 2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_pp2->get_id(), 0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void Postprocessor::begRender() const
{
    fb_beg.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Postprocessor::endRender(bool set_default_fb) const
{
    vao.bind();
    shader_blur.bind();
    sampler.bind();

    glViewport(0, 0, tex_pp1->getSize().x, tex_pp1->getSize().y);
    bool horizontal_pass = true, first_iter = true;
    int num_blurs = 2;

    for(int i = 0; i < num_blurs * 2; ++i)
    {
        if(horizontal_pass)
        {
            fb_pp1.bind();
            glUniform1f(shader_blur.getUniLocation("horizontal"), true);

            if(first_iter)
            {
                first_iter = false;
                tex_bright->bind();
            }
            else
                tex_pp2->bind();
        }
        else
        {
            fb_pp2.bind();
            glUniform1f(shader_blur.getUniLocation("horizontal"), false);
            tex_pp1->bind();
        }

        horizontal_pass = !horizontal_pass;
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glViewport(0, 0, fbSize.x, fbSize.y);
    if(set_default_fb)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Postprocessor::render(bool tone_mapping) const
{
    vao.bind();
    shader_blend.bind();
    sampler.bind(0);
    sampler.bind(1);
    tex_base->bind(0);
    tex_pp2->bind(1);

    if(tone_mapping)
        glUniform1f(shader_blend.getUniLocation("is_tone_mapping"), true);
    else
        glUniform1f(shader_blend.getUniLocation("is_tone_mapping"), false);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
