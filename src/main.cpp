#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "light/light.h"
#include "mesh/mesh.h"
#include "shader/shader.h"
#include "window/window.h"
#include "texture/texture.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow = Window();
std::vector<Mesh*> meshes;
std::vector<Shader*> shaders;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f, lastTime = 0.0f;

const char* vertexPath = "shaders/vertex.glsl";
const char* fragmentPath = "shaders/fragment.glsl";

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
       0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
       1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
       0.0f,  1.0f, 0.0f, 0.5f, 1.0f,
    };

    auto *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    meshes.push_back(obj1);

    auto *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 20, 12);
    meshes.push_back(obj2);
}

void CreateShaders() {
    auto *shader1 = new Shader();
    shader1->CreateFromFiles(vertexPath, fragmentPath);
    shaders.push_back(shader1);
}

int main() {
    mainWindow.Init();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

    brickTexture = Texture("textures/brick.png");
    dirtTexture = Texture("textures/dirt.png");

    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f);

    brickTexture.LoadTexture();
    dirtTexture.LoadTexture();

    glm::mat4 projectionMatrix = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

    // loop until window closed
    while(!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // get and handle user input events
        glfwPollEvents();

        camera.KeyControl(mainWindow.getKeys(), deltaTime);
        camera.MouseControl(mainWindow.getChangeX(), mainWindow.getChangeY());

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaders[0]->UseShader();

        mainLight.UseLight(shaders[0]->GetAmbientIntensityLocation(), shaders[0]->GetAmbientColorLocation());

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, -5.0f));
        // modelMatrix = glm::rotate(modelMatrix, toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        // modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0.5f));

        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shaders[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(shaders[0]->GetViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

        brickTexture.UseTexture();
        meshes[0]->RenderMesh();

        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.0f, -5.0f));

        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shaders[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        dirtTexture.UseTexture();
        meshes[1]->RenderMesh();

        glUseProgram(0);

        mainWindow.SwapBuffers();
    }

    return 0;
}