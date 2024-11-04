#pragma once
#include "pointlight.h"

class SpotLight : PointLight {
public:
    SpotLight();
    ~SpotLight();

    SpotLight(
        GLfloat red,
        GLfloat green,
        GLfloat blue,
        GLfloat intensity,
        GLfloat diffuseIntensity,
        GLfloat posX,
        GLfloat posY,
        GLfloat posZ,
        GLfloat dirX,
        GLfloat dirY,
        GLfloat dirZ,
        GLfloat constant,
        GLfloat linear,
        GLfloat exponent,
        GLfloat edge
    );

    void SetPositionAndDirection(glm::vec3 position, glm::vec3 direction);

    void UseLight(
        GLuint intensityUniformLocation,
        GLuint ambientColorUniformLocation,
        GLuint diffuseIntensityUniformLocation,
        GLuint positionUniformLocation,
        GLuint directionUniformLocation,
        GLuint constantLocation,
        GLuint linearLocation,
        GLuint exponentLocation,
        GLuint edgeLocation
    );

private:
    glm::vec3 mDirection{};
    GLfloat mEdge{}, mProcEdge{};
};

