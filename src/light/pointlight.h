#pragma once
#include "light.h"

class PointLight : Light {
public:
    PointLight();
    ~PointLight();

    PointLight(
        GLfloat red,
        GLfloat green,
        GLfloat blue,
        GLfloat intensity,
        GLfloat diffuseIntensity,
        GLfloat posX,
        GLfloat posY,
        GLfloat posZ,
        GLfloat constant,
        GLfloat linear,
        GLfloat exponent
    );

    void UseLight(
        GLuint intensityUniformLocation,
        GLuint ambientColorUniformLocation,
        GLuint diffuseIntensityUniformLocation,
        GLuint positionUniformLocation,
        GLuint constantLocation,
        GLuint linearLocation,
        GLuint exponentLocation
    );

private:
    glm::vec3 mPosition{};
    GLfloat mConstant{1.0f}, mLinear{0.0f}, mExponent{0.0f};
};

