//
// Created by Vlad M on 02.11.2024.
//

#include "camera.h"


Camera::Camera():
    mPosition{glm::vec3(0.0f, 0.0f, 0.0f)},
    mWorldUp{glm::vec3(0.0f, 1.0f, 0.0f)},
    mYaw{-90.0f},
    mPitch{0.0f},
    mMoveSpeed{5.0f},
    mTurnSpeed{1.0f}
{ }

Camera::Camera(
    glm::vec3 position,
    glm::vec3 worldUp,
    GLfloat yaw,
    GLfloat pitch,
    GLfloat moveSpeed,
    GLfloat turnSpeed
) {
    mPosition = position;
    mWorldUp = worldUp;
    mYaw = yaw;
    mPitch = pitch;
    mFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mMoveSpeed = moveSpeed;
    mTurnSpeed = turnSpeed;

    Update();
}

Camera::~Camera() {
}


void Camera::KeyControl(bool *keys, GLfloat deltaTime) {
    GLfloat velocity = mMoveSpeed * deltaTime;

    if (keys[GLFW_KEY_W]) {
        mPosition += mFront * velocity;
    }

    if (keys[GLFW_KEY_S]) {
        mPosition -= mFront * velocity;
    }

    if (keys[GLFW_KEY_A]) {
        mPosition -= mRight * velocity;
    }

    if (keys[GLFW_KEY_D]) {
        mPosition += mRight * velocity;
    }
}

void Camera::MouseControl(GLfloat changeX, GLfloat changeY) {
    changeX *= mTurnSpeed;
    changeY *= mTurnSpeed;

    mYaw += changeX;
    mPitch += changeY;

    if (mPitch > 89.0f) {
        mPitch = 89.0f;
    }

    if (mPitch < -89.0f) {
        mPitch = -89.0f;
    }

    Update();
}

glm::vec3 Camera::getCameraPosition() {
    return mPosition;
}

glm::vec3 Camera::getCameraDirection() {
    return glm::normalize(mFront);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::Update() {
    mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront.y = sin(glm::radians(mPitch));
    mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(mFront);

    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp = glm::normalize(glm::cross(mRight, mFront));
}
