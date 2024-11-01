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

    bool getShouldClose() { return glfwWindowShouldClose(mWindow); };
    void SwapBuffers() { glfwSwapBuffers(mWindow); };

private:
    GLFWwindow* mWindow;
    GLint mWidth, mHeight;
    GLint mBufferWidth{}, mBufferHeight{};
};
