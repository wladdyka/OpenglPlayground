#pragma once
#include <GL/glew.h>

class Material {
public:
    Material();
    Material(GLfloat intensity, GLfloat shininess);
    ~Material();

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
private:
    GLfloat mSpecularIntensity{1.0f};
    GLfloat mShininess{1.0f};
};
