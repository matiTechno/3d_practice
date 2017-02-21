#version 330

out vec4 color;

in vec2 texCoord;

uniform sampler2D base;
uniform sampler2D blur;

void main()
{
    vec3 hdr_color = texture(base, texCoord).rgb + texture(blur, texCoord).rgb;
    // tone mapping
    //hdr_color = hdr_color / (hdr_color + vec3(1));
    // gamma correction
    //hdr_color = pow(hdr_color, vec3(1.0 / 2.2));
    color = vec4(hdr_color, 1);
}
