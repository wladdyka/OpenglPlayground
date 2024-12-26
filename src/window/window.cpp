#include "window.h"

#include <cstdio>

Window::Window()
    : mWindow{},
    mWidth{1920},
    mHeight{1080},
    keys{false}
{ }

Window::~Window() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

Window::Window(GLuint width, GLuint height)
    : mWindow{},
    keys{false}
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

    //glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);

    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL Playground", nullptr, nullptr);
    if (!mWindow) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwPollEvents();

    // get buffer size information
    glfwGetFramebufferSize(mWindow, &mBufferWidth, &mBufferHeight);

    // set context for glew to use
    glfwMakeContextCurrent(mWindow);

    // handle key and mouseinput
    CreateCallbacks();
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    glfwSetWindowUserPointer(mWindow, this);

    return 0;
}

GLfloat Window::getChangeX() {
    GLfloat changeX = mChangeX;
    mChangeX = 0.0f;
    return changeX;
}

GLfloat Window::getChangeY() {
    GLfloat changeY = mChangeY;
    mChangeY = 0.0f;
    return changeY;
}

void Window::CreateCallbacks() {
    glfwSetFramebufferSizeCallback(mWindow, HandleFrameBufferSize);
    glfwSetWindowSizeCallback(mWindow, HandleWindowSize);
    glfwSetKeyCallback(mWindow, HandleKeys);
    glfwSetCursorPosCallback(mWindow, HandleMouse);
}

void Window::HandleWindowSize(GLFWwindow* window, int width, int height) {
    printf("Window::HandleWindowSize %d %d \n", width, height);
    glViewport(0, 0, width, height);
}

void Window::HandleFrameBufferSize(GLFWwindow *window, int width, int height) {
    printf("Window::HandleFrameBufferSize %d %d \n", width, height);
}

void Window::HandleKeys(GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024 && action == GLFW_PRESS) {
        theWindow->keys[key] = true;
        //printf("Pressed key %d\n", key);
    } else if (key >= 0 && key < 1024 && action == GLFW_RELEASE) {
        theWindow->keys[key] = false;
        //printf("Released key %d\n", key);
    }
}

void Window::HandleMouse(GLFWwindow *window, double xPos, double yPos) {
    auto *theWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (theWindow->mMouseFirstMoved) {
        theWindow->mLastX = xPos;
        theWindow->mLastY = yPos;
        theWindow->mMouseFirstMoved = false;
    }

    theWindow->mChangeX = xPos - theWindow->mLastX;
    theWindow->mChangeY = theWindow->mLastY - yPos;

    theWindow->mLastX = xPos;
    theWindow->mLastY = yPos;

    //printf("Mouse moved at (%f, %f)\n", theWindow->mChangeX, theWindow->mChangeY);
}
