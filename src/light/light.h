#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>

class Light {
public:
    Light();
    ~Light();

    Light(
        GLfloat red,
        GLfloat green,
        GLfloat blue,
        GLfloat intensity,
        GLfloat diffuseIntensity
    );

protected:
    glm::vec3 mColor{};
    GLfloat mAmbientIntensity{1.0f};

    GLfloat mDiffuseIntensity{1.0f};
};

