#include "light.h"

Light::Light() = default;

Light::Light(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat intensity,
    GLfloat dirX,
    GLfloat dirY,
    GLfloat dirZ,
    GLfloat diffuseIntensity
) {
    mColor = glm::vec3(red, green, blue);
    mAmbientIntensity = intensity;

    mDirection = glm::vec3(dirX, dirY, dirZ);
    mDiffuseIntensity = diffuseIntensity;
}

Light::~Light() {
}

void Light::UseLight(
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
