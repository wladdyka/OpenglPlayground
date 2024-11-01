#include "window.h"

#include <cstdio>

Window::Window()
    : mWindow{},
    mWidth{800},
    mHeight{600}
{ }

Window::~Window() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

Window::Window(GLuint width, GLuint height)
    : mWindow{}
{
    mWidth = width;
    mHeight = height;
}

int Window::Init() {
    // initialize glfw
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        glfwTerminate();
        return 1;
    }

    // setup GLFW window properties
    // opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile only, no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL Playground", nullptr, nullptr);
    if (!mWindow) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    // get buffer size information
    glfwGetFramebufferSize(mWindow, &mBufferWidth, &mBufferHeight);

    // set context for glew to use
    glfwMakeContextCurrent(mWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwDestroyWindow(mWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // setup viewport size
    glViewport(0, 0, mBufferWidth, mBufferHeight);

    return 0;
}
