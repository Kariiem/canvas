#version 330 core
precision highp float;

#define M_PI 3.1415926535897932384626433832795

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
    st = st * 2.0 - 1.0;
    float aspect_ratio = u_resolution.x/u_resolution.y;
    st.x *= aspect_ratio;
    // vec4 color = vec4(st.x, st.y, 0.0, 1.0);
    // color.x *= step(-1.0, st.x);
    // color.x *= step(1.0, st.x);
    if (dot(st.x, st.x) <= 0.00001 || dot(st.y, st.y) <= 0.00001) {
        gl_FragColor = vec4(1.0);
    }
    else {
        gl_FragColor = vec4(sin(2 * M_PI * st.x),
                        0.0,
                        0.0,
                        1.0);
    }
}
