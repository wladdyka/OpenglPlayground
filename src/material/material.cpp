#include "material.h"

Material::Material() = default;

Material::Material(GLfloat intensity, GLfloat shininess) {
    mSpecularIntensity = intensity;
    mShininess = shininess;
}

Material::~Material() = default;

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {
    glUniform1f(specularIntensityLocation, mSpecularIntensity);
    glUniform1f(shininessLocation, mShininess);
}

