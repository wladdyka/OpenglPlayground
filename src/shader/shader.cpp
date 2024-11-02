#include "shader.h"

Shader::Shader()
    : mShaderId{0},
    uniformModelId{0},
    uniformProjectionId{0},
    uniformViewModelId{0}
{
}

Shader::~Shader() {
    ClearShader();
}

void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char *vertexPath, const char *fragmentPath) {
    CompileShader(ReadFile(vertexPath).c_str(), ReadFile(fragmentPath).c_str());
}

GLuint Shader::GetProjectionLocation() {
    return uniformProjectionId;
}

GLuint Shader::GetModelLocation() {
    return uniformModelId;
}

GLuint Shader::GetViewMatrixLocation() {
    return uniformViewModelId;
}

std::string Shader::ReadFile(const char *fileName) {
    std::string content;
    std::ifstream fileStream(fileName, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to open file %s\n", fileName);
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::UseShader() {
    glUseProgram(mShaderId);
}

void Shader::ClearShader() {
    if (mShaderId != 0) {
        glDeleteProgram(mShaderId);
        mShaderId = 0;
    }

    uniformModelId = 0;
    uniformProjectionId = 0;
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode) {
    mShaderId = glCreateProgram();
    if (!mShaderId) {
        printf("Error creating shader program\n");
        return;
    }

    AddShader(mShaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(mShaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = {0};

    glLinkProgram(mShaderId);
    glGetProgramiv(mShaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(mShaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error linking program: %s\n", errorLog);
        return;
    }

    // Bind a temporary VAO for validation
    GLuint tempVao;
    glGenVertexArrays(1, &tempVao);
    glBindVertexArray(tempVao);

    glValidateProgram(mShaderId);
    glGetProgramiv(mShaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(mShaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error validating program: %s\n", errorLog);
    }

    glBindVertexArray(0); // Unbind the temporary VAO
    glDeleteVertexArrays(1, &tempVao); // Delete the temporary VAO

    uniformModelId = glGetUniformLocation(mShaderId, "modelMatrix");
    uniformProjectionId = glGetUniformLocation(mShaderId, "projectionMatrix");
    uniformViewModelId = glGetUniformLocation(mShaderId, "viewMatrix");
}

void Shader::AddShader(GLuint program, const char *shaderCode, GLenum shaderType) {
    GLuint shaderId = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(shaderId, 1, code, codeLength);
    glCompileShader(shaderId);

    GLint result = 0;
    GLchar errorLog[1024] = {0};

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shaderId, sizeof(errorLog), nullptr, errorLog);
        printf("Error compiling shader: %s\n", errorLog);
        return;
    }

    glAttachShader(program, shaderId);
}
