#pragma once
#include "light.h"

class DirectionalLight : Light {
public:
    DirectionalLight();
    ~DirectionalLight();

    DirectionalLight(
        GLfloat red,
        GLfloat green,
        GLfloat blue,
        GLfloat intensity,
        GLfloat diffuseIntensity,
        GLfloat dirX,
        GLfloat dirY,
        GLfloat dirZ
    );

    void UseLight(
        GLuint intensityUniformLocation,
        GLuint ambientColorUniformLocation,
        GLuint diffuseIntensityUniformLocation,
        GLuint directionUniformLocation
    );
private:
    glm::vec3 mDirection{};
};

