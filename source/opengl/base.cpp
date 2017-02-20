#include "base.hpp"
#include <assert.h>

GL_Base::GL_Base(std::function<void(GLuint)> deleter):
    was_moved(false),
    deleter(deleter)
{
    assert(deleter);
}

GL_Base::~GL_Base()
{
    if(!was_moved)
        deleter(id);
}

GL_Base::GL_Base(GL_Base&& rhs):
    id(rhs.id),
    was_moved(false),
    deleter(rhs.deleter)
{
    rhs.was_moved = true;
}

GL_Base& GL_Base::operator=(GL_Base&& rhs)
{
    if(this == &rhs)
        return *this;

    deleter(id);
    id = rhs.id;
    rhs.was_moved = true;
    return *this;
}
