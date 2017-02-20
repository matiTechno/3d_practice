#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D base;
uniform sampler2D blur;

void main()
{
    color = vec4(texture(base, texCoord).rgb + texture(blur, texCoord).rgb, 1);
}
