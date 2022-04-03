//
// Created by Frityet on 2022-03-24.
//

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GLFW/glfw3.h>

#include "logger.h"
#include "vector.h"

#include "shaders.h"

#define GL_VECTOR3(_x, _y, _z) VECTOR3_ut(GLVector3f_ut, _x, _y, _z)
typedef Vector3_ut(GLfloat) GLVector3f_ut;

//#define Polygon_t(...) SHARED_STRUCT(, GLVector3f_ut, points, vertices, __VA_ARGS__)
//
//typedef Polygon_t(a, b, c) Triangle_ut;

static GLVector3f_ut triangle[3];

static const Vector2_ut WINDOW_DIMENSIONS = VECTOR2(640, 480);
static GLFWwindow *win;

void render_setup(void)
{
    LOG_INFO("Setting up rendering");
    glfwInit();
    LOG_INFO("Initalised GLFW");

    //TODO: unsure if this is correct.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win = glfwCreateWindow(WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y, "WurstLink", NULL, NULL);
    if (win == NULL) {
        LOG_FATAL("Could not create window!");
        glfwTerminate();
        exit(0);
    }
    LOG_INFO("Created window!");

    glfwMakeContextCurrent(win);

    //Clearing the screen, and setting the background to a nice colour.
    glViewport(0, 0, WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(win);
    LOG_INFO("Finished render setup");

    triangle[0] = GL_VECTOR3(-0.5f, -0.5f * sqrtf(3) / 3, 0.0f);
    triangle[1] = GL_VECTOR3(0.5f, -0.5f * sqrtf(3) / 3, 0.0f);
    triangle[2] = GL_VECTOR3(0.0f, 0.5f * sqrtf(3) / 3, 0.0f);
}

void render_cleanup()
{
    glfwDestroyWindow(win);
    glfwTerminate();
}

void render_main()
{
    glfwPollEvents();

    //Technically the window will never “close” but whatever I guess.
    if (glfwWindowShouldClose(win))
        render_cleanup();
}


