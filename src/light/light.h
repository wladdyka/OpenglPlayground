#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>

class Light {
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity, GLfloat dirX, GLfloat dirY, GLfloat dirZ, GLfloat diffuseIntensity);
    ~Light();

    void UseLight(
        GLuint intensityUniformLocation,
        GLuint ambientColorUniformLocation,
        GLuint diffuseIntensityUniformLocation,
        GLuint directionUniformLocation
    );
private:
    glm::vec3 mColor{};
    GLfloat mAmbientIntensity{1.0f};

    glm::vec3 mDirection{};
    GLfloat mDiffuseIntensity{1.0f};
};

