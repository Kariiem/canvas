#ifndef TYPES_H
#define TYPES_H

#include <math.h>

typedef int32_t  i32;
typedef int64_t  i64;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;

typedef GLuint ShaderProgram;
typedef GLuint Texture2D;

typedef struct {
    f32 x, y;
} vec2;

typedef struct {
    f32 x, y, z;
} vec3;

typedef struct {
    f32 x, y, z, w;
} vec4;

#define vec_as_array(type, v)                           \
    ((type[sizeof((v))/sizeof((v).x)])(v))

typedef struct {
    f32 data[16];
} mat4;

#define I4 (mat4) { .data = { 1.0f, 0.0f, 0.0f, 0.0f,   \
                              0.0f, 1.0f, 0.0f, 0.0f,   \
                              0.0f, 0.0f, 1.0f, 0.0f,   \
                              0.0f, 0.0f, 0.0f, 1.0f}}


typedef enum {
    ATTRIB_POSITION,
    ATTRIB_TEXCOORD,
    ATTRIB_COLOR,
    ATTRIB_LAST
} Attrib;


typedef struct {
    vec2 pos;
    vec2 texcoord;
    vec4 color;
} Vertex;

typedef struct {
    GLuint vao, vbo, ebo;
    GLsizei size;
} Mesh;

typedef struct {
    f32 r, i;
} Complex;

typedef struct {
    f32 r, i, j, k;
} Quat;

mat4 mat4_translate(mat4 m, vec4 v)
{
    m.data[0 + 4*0] += v.x;
    m.data[1 + 4*1] += v.y;
    m.data[2 + 4*2] += v.z;
    m.data[3 + 4*3] += v.w;
    return m;
}

static inline
vec3 vec3_normalize(vec3 v)
{
    f32 d = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x/=d;
    v.y/=d;
    v.z/=d;
    return v;
}

Quat Quat_mult(Quat p, Quat q)
{
    Quat pq;
    pq.r = p.r * q.r - p.i * q.i - p.j * q.j - p.k * q.k;
    return pq;
}

static inline
Quat Quat_inverse(Quat q)
{
    Quat q_;
    f32 d2 = q.r * q.r + q.i * q.i + q.j * q.j + q.k * q.k;
    q_.r =  q.r/d2;
    q_.i = -q.i/d2;
    q_.j = -q.j/d2;
    q_.k = -q.k/d2;
    return q_;
}

static inline
Quat Quat_rotation_quat(f32 theta, vec3 axis)
{
    /* axis should be a unit vector */
    Quat q;
    f32 cos_half_theta = cosf(theta/2);
    f32 sin_half_theta = sinf(theta/2);
    q.r = cos_half_theta;
    q.i = axis.x * sin_half_theta;
    q.j = axis.y * sin_half_theta;
    q.k = axis.z * sin_half_theta;
    return q;
}

static inline
Complex Complex_add(Complex c1, Complex c2)
{
    Complex c;
    c.r = c1.r + c2.r;
    c.i = c1.i + c2.i;
    return c;
}

static inline
Complex Complex_mult(Complex c1, Complex c2)
{
    Complex c;
    c.r = c1.r * c2.r - c1.i * c2.i;
    c.i = c1.i * c2.r + c1.r * c2.i;
    return c;
}


#endif /* TYPES_H */
