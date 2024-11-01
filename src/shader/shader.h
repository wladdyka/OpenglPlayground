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
        GLuint GetProjectionLocation();
        GLuint GetModelLocation();

        void UseShader();
        void ClearShader();

    private:
        GLuint mShaderId, uniformProjectionId, uniformModelId;

        void CompileShader(const char* vertexCode, const char* fragmentCode);
        void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};
