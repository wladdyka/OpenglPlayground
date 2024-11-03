#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "../common.h"
#include "../light/directionallight.h"
#include "../light/pointlight.h"

class Shader {
public:
    Shader();
    ~Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexPath, const char* fragmentPath);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewMatrixLocation();
    GLuint GetAmbientIntensityLocation();
    GLuint GetAmbientColorLocation();
    GLuint GetDiffuseIntensityLocation();
    GLuint GetDirectionLocation();
    GLuint GetEyePositionLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();

    void SetDirectionalLight(DirectionalLight *light);
    void SetPointLights(PointLight *light, unsigned int lightCount);

    std::string ReadFile(const char* fileName);

    void UseShader();
    void ClearShader();

private:
    int mPointLightCount{0};

    GLuint mShaderId, uniformProjectionId, uniformModelId, uniformViewModelId;
    GLuint uniformEyePositionId, uniformSpecularIntensityId, uniformShininessId;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformDirection;
    } uniformDirectionalLight;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight [MAX_POINT_LIGHTS];
    GLuint uniformPointLightCount;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};
