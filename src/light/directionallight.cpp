#include "directionallight.h"

DirectionalLight::DirectionalLight() : Light() { }

DirectionalLight::DirectionalLight(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat intensity,
    GLfloat diffuseIntensity,
    GLfloat dirX,
    GLfloat dirY,
    GLfloat dirZ
) : Light(red, green, blue, intensity, diffuseIntensity) {
    mDirection = glm::vec3(dirX, dirY, dirZ);
}

DirectionalLight::~DirectionalLight() = default;

void DirectionalLight::UseLight(
    GLuint intensityUniformLocation,
    GLuint ambientColorUniformLocation,
    GLuint diffuseIntensityUniformLocation,
    GLuint directionUniformLocation
) {
    glUniform3f(ambientColorUniformLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(intensityUniformLocation, mAmbientIntensity);
    glUniform3f(directionUniformLocation, mDirection.x, mDirection.y, mDirection.z);
    glUniform1f(diffuseIntensityUniformLocation, mDiffuseIntensity);
}
