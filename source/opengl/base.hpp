#ifndef BASE_HPP
#define BASE_HPP

#include "../glad/glad.h"
#include <functional>

class GL_Base
{
public:
    GL_Base(std::function<void(GLuint)> deleter);
    ~GL_Base();
    // no copy operations
    GL_Base(const GL_Base&) = delete;
    GL_Base& operator=(const GL_Base&) = delete;

    GL_Base(GL_Base&& rhs);
    GL_Base& operator=(GL_Base&& rhs);

protected:
    GLuint id;

private:
    bool was_moved;
    std::function<void(GLuint)> deleter;
};

#endif // BASE_HPP
