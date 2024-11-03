#include "light.h"

Light::Light() = default;

Light::Light(
    GLfloat red,
    GLfloat green,
    GLfloat blue,
    GLfloat intensity,
    GLfloat diffuseIntensity
) {
    mColor = glm::vec3(red, green, blue);
    mAmbientIntensity = intensity;
    mDiffuseIntensity = diffuseIntensity;
}

Light::~Light() = default;