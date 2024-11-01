#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh/mesh.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshes;

GLuint mShaderId, modelMatrixUniformVarId, projectionMatrixUniformVarId;

// vertex shader
static const char* vertexShaderSource = ""
"#version 330\n"
"layout (location = 0) in vec3 pos;\n"
"out vec4 vertexColor;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"void main() {\n"
"gl_Position = projectionMatrix * modelMatrix * vec4(pos, 1.0);\n"
"vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);"
"}";

// fragment shader
static const char* fragmentShaderSource = ""
"#version 330\n"
"in vec4 vertexColor;\n"
"out vec4 color;\n"
"void main() {\n"
"color = vertexColor;\n"
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

    // Bind a temporary VAO for validation
    GLuint tempVao;
    glGenVertexArrays(1, &tempVao);
    glBindVertexArray(tempVao);

    glValidateProgram(mShaderId);
    glGetProgramiv(mShaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(mShaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error validating program: %s\n", errorLog);
    }

    glBindVertexArray(0); // Unbind the temporary VAO
    glDeleteVertexArrays(1, &tempVao); // Delete the temporary VAO

    modelMatrixUniformVarId = glGetUniformLocation(mShaderId, "modelMatrix");
    projectionMatrixUniformVarId = glGetUniformLocation(mShaderId, "projectionMatrix");
}

void CreateTriangle() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f,
       0.0f, -1.0f, 1.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshes.push_back(obj1);
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

    glEnable(GL_DEPTH_TEST);

    // setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    glm::mat4 projectionMatrix = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

    // loop until window closed
    while(!glfwWindowShouldClose(mainWindow)) {
        // get and handle user input events
        glfwPollEvents();

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(mShaderId);

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
        modelMatrix = glm::rotate(modelMatrix, toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0.5f));

        glUniformMatrix4fv(modelMatrixUniformVarId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(projectionMatrixUniformVarId, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        for (auto mesh : meshes) {
         mesh->RenderMesh();
        }

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}