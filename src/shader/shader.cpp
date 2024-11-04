#include "shader.h"

#include "../light/spotlight.h"

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

GLuint Shader::GetAmbientIntensityLocation() {
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation() {
    return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetDiffuseIntensityLocation() {
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation() {
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetEyePositionLocation() {
    return uniformEyePositionId;
}

GLuint Shader::GetSpecularIntensityLocation() {
    return uniformSpecularIntensityId;
}

GLuint Shader::GetShininessLocation() {
    return uniformShininessId;
}

void Shader::SetDirectionalLight(DirectionalLight *light) {
    light->UseLight(
        uniformDirectionalLight.uniformAmbientIntensity,
        uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity,
        uniformDirectionalLight.uniformDirection
    );
}

void Shader::SetPointLights(PointLight *light, unsigned int lightCount) {
    if (lightCount > MAX_POINT_LIGHTS) {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        light[i].UseLight(
            uniformPointLight[i].uniformAmbientIntensity,
            uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity,
            uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant,
            uniformPointLight[i].uniformLinear,
            uniformPointLight[i].uniformExponent
        );
    }
}

void Shader::SetSpotLights(SpotLight *light, unsigned int lightCount) {
    if (lightCount > MAX_SPOT_LIGHTS) {
        lightCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(uniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        light[i].UseLight(
            uniformSpotLight[i].uniformAmbientIntensity,
            uniformSpotLight[i].uniformColor,
            uniformSpotLight[i].uniformDiffuseIntensity,
            uniformSpotLight[i].uniformPosition,
            uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant,
            uniformSpotLight[i].uniformLinear,
            uniformSpotLight[i].uniformExponent,
            uniformSpotLight[i].uniformEdge
        );
    }
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

    uniformDirectionalLight.uniformColor = glGetUniformLocation(mShaderId, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(mShaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(mShaderId, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(mShaderId, "directionalLight.direction");

    uniformEyePositionId = glGetUniformLocation(mShaderId, "eyePosition");
    uniformSpecularIntensityId = glGetUniformLocation(mShaderId, "material.specularIntensity");
    uniformShininessId = glGetUniformLocation(mShaderId, "material.shininess");

    uniformPointLightCount = glGetUniformLocation(mShaderId, "pointLightCount");
    for(unsigned int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locationBuffer[100] = {'\0'};

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(mShaderId, locationBuffer);
    }

    uniformSpotLightCount = glGetUniformLocation(mShaderId, "spotLightCount");
    for(unsigned int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locationBuffer[100] = {'\0'};

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(mShaderId, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(mShaderId, locationBuffer);
    }
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
