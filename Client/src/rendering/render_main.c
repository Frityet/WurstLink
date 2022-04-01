//
// Created by Frityet on 2022-03-24.
//

#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "logger.h"
#include "vector.h"

#include "shaders.h"

static const Vector2_t WINDOW_DIMENSIONS = VECTOR2(640, 480);

static GLFWwindow *win;

void render_setup(void)
{
    glfwInit();

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

    glfwMakeContextCurrent(win);
    gladLoadGL();

    //Clearing the screen, and setting the background to a nice colour.
    glViewport(0, 0, WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(win);

    LOG_DEBUG("Created window");
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


