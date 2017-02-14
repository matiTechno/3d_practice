#include "buffers.hpp"

GLuint VAO::bound_id = 0;

VAO::VAO()
{
    glGenVertexArrays(1, &id);

}

VAO::~VAO()
{
    if(!was_moved)
        glDeleteVertexArrays(1, &id);
}

void VAO::bind() const
{
    if(bound_id != id)
    {
        bound_id = id;
        glBindVertexArray(id);
    }
}

BO::BO()
{
    glGenBuffers(1, &id);
}

BO::~BO()
{
    if(!was_moved)
        glDeleteBuffers(1, &id);
}

void BO::bind(GLenum target) const
{
    glBindBuffer(target, id);
}

FBO::FBO()
{
    glGenFramebuffers(1, &id);
}

FBO::~FBO()
{
    if(!was_moved)
        glDeleteFramebuffers(1, &id);
}

void FBO::bind(GLenum target) const
{
    glBindFramebuffer(target, id);
}
