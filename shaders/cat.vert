#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec4 vColor;

out vec2 TexCoord;
out vec4 ourColor;


void main()
{
    gl_Position = vec4(vPos, 0.0, 1.0);
    ourColor = vColor;
    TexCoord = vTexCoord;
}
