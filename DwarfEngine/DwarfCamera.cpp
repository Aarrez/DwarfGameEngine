#include "DwarfCamera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace Dwarf::Camera;

DwarfCamera::DwarfCamera(std::shared_ptr<DwarfShader> shader) : shader(shader){
    view = mat4(1.0f);
    cameraPos = glm::vec3(0,0,3.0f);
    cameraDirection = glm::vec3(0, 0, 0);
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);
    view = glm::lookAt(cameraPos, cameraTarget, up);
}

void DwarfCamera::RotateCameraWithTime(
    double time,
    const float radius,
    const vec3 direction) {

    float camX = sin(time) * radius;
    float camY = cos(time) * radius;
    view = lookAt(vec3(camX, 0, camY), vec3(0, 0, 0), direction);
    shader->SetMatrix4("view", 1, GL_FALSE, view);
}

void DwarfCamera::MoveCamera(glm::vec3 direction, float speed) {
    view = lookAt(direction * speed, cameraTarget, cameraUp);
    shader->SetMatrix4("view", 1, GL_FALSE, view);
}

void DwarfCamera::RotateCamera(glm::vec3 axis, float angle) {

}



