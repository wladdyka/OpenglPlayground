#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>

class Light {
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity);
    ~Light();

    void UseLight(GLfloat intensityUniformLocation, GLfloat ambientColorUniformLocation);
private:
    glm::vec3 mColor{};
    GLfloat mAmbientIntensity{1.0f};
};

