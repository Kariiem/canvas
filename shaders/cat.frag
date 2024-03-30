#version 330 core


out vec4 FragColor;

in vec2 TexCoord;
in vec4 ourColor;

// texture samplers
uniform sampler2D tex1;
uniform sampler2D tex2;

uniform float mixer_loc;

void main()
{
    FragColor = mix(texture(tex1, TexCoord),
                    texture(tex2, TexCoord),
                    mixer_loc);
    FragColor = mix(FragColor, ourColor, 0.5);
}
