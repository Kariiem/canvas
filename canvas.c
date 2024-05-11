#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "types.h"
#include "readfile.h"
#include "macros.h"

#define DEFAULT_VERT_SHADER "./shaders/canvas.vert"
#define DEFAULT_FRAG_SHADER "./shaders/canvas.frag"


bool load_shader(ShaderProgram *id,
                 char const *vert_shader_path,
                 char const *frag_shader_path)
{
    char const *vert_source = read_file(vert_shader_path);
    char const *frag_source = read_file(frag_shader_path);


    int vert_success;
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_source, NULL);
    glCompileShader(vert);
    CHECK_SHADER_ERRORS(Shader, GL_COMPILE_STATUS, vert, vert_success);

    int frag_success;
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_source, NULL);
    glCompileShader(frag);
    CHECK_SHADER_ERRORS(Shader, GL_COMPILE_STATUS, frag, frag_success);

    int prog_success;
    ShaderProgram program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    CHECK_SHADER_ERRORS(Program, GL_LINK_STATUS, program, prog_success);
    *id = program; /* bind the program id  */

    free((void*) vert_source);
    free((void*) frag_source);
    glDeleteShader(vert);
    glDeleteShader(frag);

    return vert_success && frag_success && prog_success;
}


typedef struct {
    GLuint vao;
    ShaderProgram program;
    bool pause;
    double time;
} Renderer;

static Renderer *rptr;


void renderer_reload(Renderer *r)
{
    glDeleteProgram(r->program);
    r->time = 0.0;
    ShaderProgram p;
    if(load_shader(&p,
                   DEFAULT_VERT_SHADER,
                   DEFAULT_FRAG_SHADER)) {
        r->program = p;
    }
}


void render(Renderer const *r)
{
    glUseProgram(r->program);
}


void framebuffer_resize_callback(GLFWwindow *win, int width, int height)
{
    IGNORE(win);
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    IGNORE(scancode);
    IGNORE(mods);
    if (action == GLFW_PRESS) {
        switch(key) {
        case GLFW_KEY_Q:
            glfwSetWindowShouldClose(window, true);
            break;
        case GLFW_KEY_P:
            rptr->pause = !rptr->pause;
            break;
        case GLFW_KEY_R:
            renderer_reload(rptr);
            break;
        default:
            eprintf("Unhandled key\n");
        }
    }
}


int main(void)
{
    GLFWwindow *window = NULL;
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(800, 600, "game", NULL, NULL);
        if (!window) {
            eprintf("Failed to create window");
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
        glfwSetKeyCallback(window, key_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            eprintf("Failed to inialize GLAD");
            return -1;
        }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderer r;
    {
        r.pause = 0;
        r.program = 0;
        r.time = 0.0;
        glGenVertexArrays(1,&r.vao);
        glBindVertexArray(r.vao);
        rptr = &r;
    }


    if(!load_shader(&rptr->program,
                    DEFAULT_VERT_SHADER,
                    DEFAULT_FRAG_SHADER)) {
        eprintf("Shader error");
        exit(1);
    }

    double current_time = glfwGetTime();
    double new_time = current_time;
    double frame_time = 0.0f;
    double delta_time = 1.0f/60.0f;
    double frame_time_accumulator = 0.0f;


    GLint u_time = glGetUniformLocation(rptr->program, "u_time");
    GLint u_resolution = glGetUniformLocation(rptr->program, "u_resolution");
    GLint u_mouse = glGetUniformLocation(rptr->program, "u_mouse");

    struct {
        GLint x,y, width, height;
    } viewport = {0};

    struct {
        double x,y;
    } mouse = {0};

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(u_time, rptr->time);

        glGetIntegerv(GL_VIEWPORT, (GLint *)&viewport);
        glUniform2f(u_resolution, viewport.width, viewport.height);

        glfwGetCursorPos(window, &mouse.x, &mouse.y);
        glUniform2f(u_mouse, mouse.x, mouse.y);

        render(rptr);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();

        /* time */
        new_time = glfwGetTime();
        frame_time = new_time - current_time;

        current_time = new_time;

        /* discretize frame time into delta time steps
         * this requires an accumulator that stores
         * the frame time and changes with a step of
         * delta time, the remaining time is carried to the next frame
         */
        frame_time_accumulator += frame_time;
        while(frame_time_accumulator > delta_time) {
            frame_time_accumulator -= delta_time;

            if (!rptr->pause) {
                rptr->time += delta_time;
                printf("INFO:: frame_time = %f\n", frame_time);
            }

        }

    }
    glDeleteProgram(rptr->program);
    return 0;
}
