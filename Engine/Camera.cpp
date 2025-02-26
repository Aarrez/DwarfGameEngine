#include "Camera.h"

#include <utility>



using namespace Engine;

Camera::Camera(){
    view = mat4(1.0f);
    cameraPos = vec3(0,0,20.0f);
    cameraDirection = glm::vec3(0, 0, -1);
    /*cameraDirection = glm::normalize(cameraPos - cameraTarget);*/
    cameraRight = normalize(cross(up, cameraDirection));
    cameraUp = glm::vec3(0, 1.0f, 0);
    cameraFront =  glm::vec3(0, 0, -1.0f);
    view = glm::lookAt(cameraPos, cameraTarget, up);
}


void Camera::MoveCamera(glm::vec3 direction, float speed, glm::mat4* view) {
    cameraPos += direction * speed;
    *view = lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
}

void Camera::SetCameraPos(const glm::vec3& pos) {
    cameraPos = pos;
}

vec3 Camera::GetCameraPos() {
    return cameraPos;
}

void Camera::SetCameraRotation(glm::vec3 rotation) {
    cameraDirection = rotation;
}

glm::vec3 Camera::GetCameraRotation() {
    return cameraDirection;
}

void Camera::RotateCamera(double Xpos, double Ypos, bool first) {
    if (first) {
        lastX = Xpos;
        lastY = Ypos;
    }
    offsetX = Xpos - lastX;
    offsetY = lastY - Ypos;
    lastX = Xpos;
    lastY = Ypos;

    offsetX *= sensitivity;
    offsetY *= sensitivity;
    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    vec3 tempdir;
    tempdir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    tempdir.y = sin(glm::radians(pitch));
    tempdir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection = glm::normalize(tempdir);
}



