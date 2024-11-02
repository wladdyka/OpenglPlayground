#include "light.h"

Light::Light(): mColor {glm::vec3(1.0f, 1.0f, 1.0f)} {
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity) {
    mColor = glm::vec3(red, green, blue);
    mAmbientIntensity = intensity;
}

Light::~Light() {
}

void Light::UseLight(GLfloat intensityUniformLocation, GLfloat ambientColorUniformLocation) {
    glUniform3f(ambientColorUniformLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(intensityUniformLocation, mAmbientIntensity);
}
