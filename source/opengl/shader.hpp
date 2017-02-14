#ifndef SHADER_HPP
#define SHADER_HPP

#include "base.hpp"
#include <string>
#include <unordered_map>

class ShaderPart: public GL_Base
{
public:
    ShaderPart(GLenum shader_type);
    ~ShaderPart();
    ShaderPart(ShaderPart&&) = default;
    ShaderPart& operator=(ShaderPart&&) = default;

    GLuint get_id() const;
};

class Shader: public GL_Base
{
public:
    Shader(const std::string& vertex_d, const std::string& fragment_d, const std::string& geometry_d, bool is_from_file);
    ~Shader();
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    void bind() const;
    GLint getUniLocation(const std::string& uniform_name);

private:
    static GLuint bound_id;
    std::unordered_map<std::string, GLint> uniform_locations;

    std::string load_source_from_file(const std::string& filename);
    void load_uniform_locations();
    ShaderPart compile_shader(const std::string& source, GLenum shader_type);
    bool isError(bool isProgram, GLuint id, GLenum flag, const std::string& error_msg);
};

#endif // SHADER_HPP
