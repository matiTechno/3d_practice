#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(position, 1);
}
