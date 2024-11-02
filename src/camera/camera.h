#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 worldUp, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed);
    ~Camera();

    void KeyControl(bool* keys, GLfloat deltaTime);
    void MouseControl(GLfloat changeX, GLfloat changeY);
    glm::mat4 GetViewMatrix();

private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;

    GLfloat mYaw;
    GLfloat mPitch;

    GLfloat mMoveSpeed;
    GLfloat mTurnSpeed;

    void Update();
};

