#version 330

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 color_bloom;

void main()
{
    color = vec4(1, 1, 1, 0.1);
    color_bloom = vec4(0, 0, 0, 0);
}
