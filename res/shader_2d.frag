#version 330

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 color_bloom;

in vec2 texCoord;

uniform sampler2D sampl;

uniform vec4 spriteColor;
uniform bool isBloom;
uniform bool is_tex_sprite;
uniform bool is_text;

void main()
{
    if(is_tex_sprite)
    {
        color = texture(sampl, texCoord) * spriteColor;
    }
    else if(is_text)
    {
        color = spriteColor;
    }
    else
    {
        if(texture(sampl, texCoord).r >= 0.5)
            color = vec4(0, 0, 0, 0);
        else
            color = spriteColor;
    }

    if(isBloom)
    {
        color_bloom = color;
    }
    else
        discard;
}
