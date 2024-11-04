#include "spotlight.h"

#include <math.h>
#include <glm/trigonometric.hpp>
#include <glm/ext/quaternion_geometric.hpp>

SpotLight::SpotLight() : PointLight() { }

SpotLight::~SpotLight() = default;

SpotLight::SpotLight(
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat intensity, GLfloat diffuseIntensity,
    GLfloat posX, GLfloat posY, GLfloat posZ,
    GLfloat dirX, GLfloat dirY, GLfloat dirZ, GLfloat constant,
    GLfloat linear, GLfloat exponent, GLfloat edge
) : PointLight(red, green, blue, intensity, diffuseIntensity,
    posX, posY, posZ, constant, linear, exponent
) {
    mDirection = glm::normalize(glm::vec3(dirX, dirY, dirZ));
    mEdge = edge;
    mProcEdge = cosf(glm::radians(mEdge));
}

void SpotLight::SetPositionAndDirection(glm::vec3 position, glm::vec3 direction) {
    mDirection = direction;
    mPosition = position;
}

void SpotLight::UseLight(
    GLuint intensityUniformLocation,
    GLuint ambientColorUniformLocation,
    GLuint diffuseIntensityUniformLocation,
    GLuint positionUniformLocation,
    GLuint directionUniformLocation,
    GLuint constantLocation,
    GLuint linearLocation,
    GLuint exponentLocation,
    GLuint edgeLocation
) {
    glUniform3f(ambientColorUniformLocation, mColor.x, mColor.y, mColor.z);
    glUniform1f(intensityUniformLocation, mAmbientIntensity);
    glUniform1f(diffuseIntensityUniformLocation, mDiffuseIntensity);

    glUniform3f(positionUniformLocation, mPosition.x, mPosition.y, mPosition.z);
    glUniform1f(constantLocation, mConstant);
    glUniform1f(linearLocation, mLinear);
    glUniform1f(exponentLocation, mExponent);

    glUniform3f(directionUniformLocation, mDirection.x, mDirection.y, mDirection.z);
    glUniform1f(edgeLocation, mProcEdge);
}
