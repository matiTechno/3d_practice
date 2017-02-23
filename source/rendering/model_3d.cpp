#include "model_3d.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>

Model_3D::Model_3D(const std::string& filename):
    material{glm::vec3(1.f, 1.f, 1.f), 0.f, 0.f},
    position(0.f, 0.f, 0.f),
    scale(1.f, 1.f, 1.f),
    rotation(0.f, 0.f, 0.f)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename.c_str(),
                                             aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

    if(!scene)
        throw std::runtime_error("assimp failed to load: " + filename);

    if(scene->mNumMeshes != 1)
        throw std::runtime_error("Model_3D: only one mesh per model is supported: " + filename);

    aiMesh* mesh = scene->mMeshes[0];

    if(!mesh->HasNormals())
        throw std::runtime_error("Model_3D: only models with normal vectors are supported: " + filename);

    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    std::vector<GLuint> indices;
    indices.reserve(mesh->mNumFaces * mesh->mFaces->mNumIndices);

    for(std::size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        vertices.push_back(std::move(vertex));
    }
    for(std::size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(std::size_t j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    this->mesh = std::make_unique<Mesh>(vertices, indices);
}
