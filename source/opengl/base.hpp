#ifndef BASE_HPP
#define BASE_HPP

#include "../glad/glad.h"

class GL_Base
{
public:
    GL_Base():
        was_moved(false)
    {}

    // no copy operations
    GL_Base(const GL_Base&) = delete;
    GL_Base& operator=(const GL_Base&) = delete;

    GL_Base(GL_Base&& rhs):
        id(rhs.id),
        was_moved(false)
    {
        rhs.was_moved = true;
    }

    GL_Base& operator=(GL_Base&& rhs)
    {
        id = rhs.id;
        rhs.was_moved = true;
        return *this;
    }

protected:
    GLuint id;
    bool was_moved;
};

#endif // BASE_HPP
