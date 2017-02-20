#ifndef RENDERER_2D_HPP
#define RENDERER_2D_HPP

#include <glm/fwd.hpp>
class Sprite;
class Text;
#include "../opengl/buffers.hpp"
#include "../opengl/texture.hpp"
#include "../opengl/shader.hpp"
#include <memory>

// to do: batching
class Renderer_2D
{
public:
    Renderer_2D();

    void render(const Sprite& sprite) const;
    void render(const Text& text) const;

    void load_projection(const glm::mat4& matrix);

private:
    static bool isCurrent;

    VAO vao;
    BO vbo;
    Sampler sampler;
    std::unique_ptr<Shader> shader;
};

#endif // RENDERER_2D_HPP
