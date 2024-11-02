#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();

    Window(GLuint width, GLuint height);
    int Init();

    GLint getBufferWidth() { return mBufferWidth; };
    GLint getBufferHeight() { return mBufferHeight; };

    bool* getKeys() { return keys; }
    GLfloat getChangeX();
    GLfloat getChangeY();

    bool getShouldClose() { return glfwWindowShouldClose(mWindow); };
    void SwapBuffers() { glfwSwapBuffers(mWindow); };

private:
    GLFWwindow* mWindow;
    GLint mWidth, mHeight;
    GLint mBufferWidth{}, mBufferHeight{};

    bool keys[1024];

    GLfloat mLastX{0}, mLastY{0}, mChangeX{0}, mChangeY{0};
    bool mMouseFirstMoved{true};

    void CreateCallbacks();
    static void HandleKeys(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};
