#version 330 core

out vec2 uv;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

void main()
{
    uv.x = gl_VertexID & 0x1;
    uv.y = (gl_VertexID >> 1) & 0x1;

    gl_Position = vec4(2 * uv.xy - 1,
                       0.0,
                       1.0);
}
