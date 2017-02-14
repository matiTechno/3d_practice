#ifndef BUFFERS_HPP
#define BUFFERS_HPP

#include "base.hpp"

// vertex array object
class VAO: public GL_Base
{
public:
    VAO();
    ~VAO();
    VAO(VAO&&) = default;
    VAO& operator=(VAO&&) = default;

    void bind() const;

private:
    static GLuint bound_id;
};

// buffer object
class BO: public GL_Base
{
public:
    BO();
    ~BO();
    BO(BO&&) = default;
    BO& operator=(BO&&) = default;

    void bind(GLenum target) const;
};

// framebuffer object
class FBO: public GL_Base
{
public:
    FBO();
    ~FBO();
    FBO(FBO&&) = default;
    FBO& operator=(FBO&&) = default;

    void bind(GLenum target) const;
};

#endif // BUFFERS_HPP
