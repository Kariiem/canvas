#version 330 core

void main()
{
    float x = (gl_VertexID & 1);
    float y = ((gl_VertexID >> 1) & 1);
    gl_Position = vec4(x - 0.5,
                       y - 0.5,
                       0.0,
                       1.0);


#if 0
    float x = gl_VertexID & 1;
    float y = ((gl_VertexID >> 1) & 1);
    gl_Position = vec4(x - 0.5,
                       y - 0.5,
                       0.0,
                       1.0);
#endif
}

// 0 ( 0.5,  0.5)
// 1 ( 0.5, -0.5)
// 2 (-0.5, -0.5)
// 3 (-0.5,  0.5)
