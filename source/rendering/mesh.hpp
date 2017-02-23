#ifndef MESH_HPP
#define MESH_HPP

#include "../opengl/buffers.hpp"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

    VAO vao;
    BO vbo, ebo;
    std::size_t num_indices;
};

#endif // MESH_HPP
