#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

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

    std::string ReadFile(const char* fileName);

    void UseShader();
    void ClearShader();

private:
    GLuint mShaderId, uniformProjectionId, uniformModelId, uniformViewModelId;
    GLuint uniformAmbientColorId, uniformAmbientIntensityId, uniformDiffuseIntensityId, uniformDirectionId;
    GLuint uniformEyePositionId, uniformSpecularIntensityId, uniformShininessId;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};
