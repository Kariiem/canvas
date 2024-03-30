#version 330 core

in vec2 uv;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

// void main()
// {
//     gl_FragColor = vec4(uv * cos(u_time) * 0.5 + 0.5,
//                         0.5 * sin(u_time) + 0.5,
//                         1.0);
// }

void main()
{
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    st *= 20.0;
    st -= 15.0;

    float l = sin(u_time * length(st));
    float c = cos(st.x);
    float s = sin(st.y);

    vec3 color = vec3(0.0);
    color += vec3(s, c, l);
    color += vec3(-c, -s, 0.0);
    gl_FragColor = vec4(color, 1.0);
}
