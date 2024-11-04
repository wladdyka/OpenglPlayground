#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "light/directionallight.h"
#include "material/material.h"
#include "mesh/mesh.h"
#include "model/model.h"
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
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackHawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f, lastTime = 0.0f;

const char* vertexPath = "shaders/vertex.glsl";
const char* fragmentPath = "shaders/fragment.glsl";

void calcAverageNormals(
    unsigned int * indices,
    unsigned int indiceCount,
    GLfloat * vertices,
    unsigned int verticeCount,
    unsigned int vLength,
    unsigned int normalOffset
) {
    for (size_t i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        //	x      y      z			u	  v			nx	  ny    nz
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,	0.5f, 0.0f,	   0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,	1.0f, 0.0f,	   0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,	0.5f, 1.0f,	   0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5);

    auto *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshes.push_back(obj1);

    auto *obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshes.push_back(obj2);

    auto *obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshes.push_back(obj3);
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
    brickTexture.LoadTextureAlpha();

    dirtTexture = Texture("textures/dirt.png");
    dirtTexture.LoadTextureAlpha();

    plainTexture = Texture("textures/plain.png");
    plainTexture.LoadTextureAlpha();

    shinyMaterial = Material(4.0f, 256);
    dullMaterial = Material(0.3f, 4);

    xwing = Model();
    xwing.LoadModel("models/x-wing.obj");

    blackHawk = Model();
    blackHawk.LoadModel("models/uh60.obj");

    mainLight = DirectionalLight(
    1.0f, 1.0f, 1.0f,
    0.2f, 0.4f,
    0.0f, 0.0f, -1.0f
    );

    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(
    0.0f, 0.0f, 1.0f,
    0.4f, 0.3f,
    4.0f, 0.0f, 0.0f,
    0.3f, 0.2f, 0.1f
    );
    pointLightCount++;
    pointLights[1] = PointLight(
    0.0f, 1.0f, 0.0f,
    0.3f, 0.3f,
    -4.0f, 2.0f, 0.0f,
    0.3f, 0.1f, 0.1f
    );
    pointLightCount++;

    unsigned int spotLightCount = 0;
    spotLights[0] = SpotLight(
    1.0f, 1.0f, 1.0f,
    0.0f, 2.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    20.0f
    );
    spotLightCount++;

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
        shaders[0]->SetDirectionalLight(&mainLight);
        shaders[0]->SetPointLights(pointLights, pointLightCount);
        shaders[0]->SetSpotLights(spotLights, spotLightCount);

        auto cameraPos = camera.getCameraPosition();
        spotLights[0].SetPositionAndDirection(cameraPos, camera.getCameraDirection());

        glUniformMatrix4fv(shaders[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(shaders[0]->GetViewMatrixLocation(), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniform3f(shaders[0]->GetEyePositionLocation(), cameraPos.x, cameraPos.y, cameraPos.z);

        // first pyramid
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, -5.0f));
        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        brickTexture.UseTexture();
        shinyMaterial.UseMaterial(shaders[0]->GetSpecularIntensityLocation(), shaders[0]->GetShininessLocation());
        meshes[0]->RenderMesh();

        // second pyramid
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.0f, 0.0f, -5.0f));
        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        dirtTexture.UseTexture();
        dullMaterial.UseMaterial(shaders[0]->GetSpecularIntensityLocation(), shaders[0]->GetShininessLocation());
        meshes[1]->RenderMesh();

        // xwing object
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-7.0f, 0.0f, 10.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.006f, 0.006f, 0.006f));
        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        shinyMaterial.UseMaterial(shaders[0]->GetSpecularIntensityLocation(), shaders[0]->GetShininessLocation());
        xwing.RenderModel();

        // blackHawk object
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.0f, 0.0f, 4.0f));
        modelMatrix = glm::rotate(modelMatrix, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        shinyMaterial.UseMaterial(shaders[0]->GetSpecularIntensityLocation(), shaders[0]->GetShininessLocation());
        blackHawk.RenderModel();

        // floor object
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
        glUniformMatrix4fv(shaders[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        dirtTexture.UseTexture();
        shinyMaterial.UseMaterial(shaders[0]->GetSpecularIntensityLocation(), shaders[0]->GetShininessLocation());
        meshes[2]->RenderMesh();

        glUseProgram(0);
        mainWindow.SwapBuffers();
    }

    return 0;
}