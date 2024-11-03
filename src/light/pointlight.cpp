//
// Created by Vlad M on 03.11.2024.
//

#include "pointlight.h"

PointLight::PointLight() : Light() { }

PointLight::~PointLight() = default;

PointLight::PointLight(
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
) : Light(red, green, blue, intensity, diffuseIntensity) {
    mPosition = glm::vec3(posX, posY, posZ);
    mConstant = constant;
    mLinear = linear;
    mExponent = exponent;
}

void PointLight::UseLight(
    GLuint intensityUniformLocation,
    GLuint ambientColorUniformLocation,
    GLuint diffuseIntensityUniformLocation,
    GLuint positionUniformLocation,
    GLuint constantLocation,
    GLuint linearLocation,
    GLuint exponentLocation
) {
    glUniform3f(ambientColorUniformLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(intensityUniformLocation, mAmbientIntensity);
    glUniform1f(diffuseIntensityUniformLocation, mDiffuseIntensity);

    glUniform3f(positionUniformLocation, mPosition.x, mPosition.y, mPosition.z);
    glUniform1f(constantLocation, mConstant);
    glUniform1f(linearLocation, mLinear);
    glUniform1f(exponentLocation, mExponent);
}
