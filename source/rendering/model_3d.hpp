#ifndef MODEL_3D_HPP
#define MODEL_3D_HPP

#include <glm/vec3.hpp>
#include <memory>
#include "mesh.hpp"
#include <string>

struct Material
{
    glm::vec3 diffuse_c;
    float specular_intensity;
    float shininess;
};


// models with multiple meshes are not supported for now
class Model_3D
{
public:
    friend class Renderer_3D;

    Model_3D(const std::string& filename);

    Material material;
    glm::vec3 position, scale, rotation;

private:
    std::unique_ptr<Mesh> mesh;
};

#endif // MODEL_3D_HPP
