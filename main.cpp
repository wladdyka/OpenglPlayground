#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint vaoId, vboId, mShaderId, modelMatrixUniformVarId;

bool isDirectionRight = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleStepIncrement = 0.005f;

// vertex shader
static const char* vertexShaderSource = ""
"#version 330\n"
"layout (location = 0) in vec3 pos;\n"
"uniform mat4 modelMatrix;\n"
"void main() {\n"
"gl_Position = modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);\n"
"}";

// fragment shader
static const char* fragmentShaderSource = ""
"#version 330\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}";

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(shaderId, 1, code, codeLength);
    glCompileShader(shaderId);

    GLint result = 0;
    GLchar errorLog[1024] = {0};

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error compiling shader: %s\n", errorLog);
        return;
    }

    glAttachShader(program, shaderId);
}

void CompileShaders() {
    mShaderId = glCreateProgram();
    if (!mShaderId) {
        printf("Error creating shader program\n");
        return;
    }

    AddShader(mShaderId, vertexShaderSource, GL_VERTEX_SHADER);
    AddShader(mShaderId, fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = {0};

    glLinkProgram(mShaderId);
    glGetProgramiv(mShaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(mShaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking program: %s\n", errorLog);
        return;
    }

    glValidateProgram(mShaderId);
    glGetProgramiv(mShaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(mShaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error validating program: %s\n", errorLog);
        return;
    }

    modelMatrixUniformVarId = glGetUniformLocation(mShaderId, "modelMatrix");
}

void CreateTriangle() {
    GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f,
      1.0f, -1.0f, 0.0f,
      0.0f,  1.0f, 0.0f,
    };

    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main() {
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Playground", nullptr, nullptr);
    if (!mainWindow) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    // get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set context for glew to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // loop until window closed
    while(!glfwWindowShouldClose(mainWindow)) {
        // get and handle user input events
        glfwPollEvents();

        if (isDirectionRight) {
            triangleOffset += triangleStepIncrement;
        } else {
            triangleOffset -= triangleStepIncrement;
        }

        if (abs(triangleOffset) > triangleMaxOffset) {
            isDirectionRight = !isDirectionRight;
        }

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vaoId);

        glUseProgram(mShaderId);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(triangleOffset, 0.0f, 0.0f));

        glUniformMatrix4fv(modelMatrixUniformVarId, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}