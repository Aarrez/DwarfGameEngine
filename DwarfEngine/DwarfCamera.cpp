#include "DwarfCamera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace Dwarf::Camera;

DwarfCamera::DwarfCamera(std::shared_ptr<DwarfShader> shader) : shader(shader){
    view = mat4(1.0f);
    cameraPos = vec3(0,0,3.0f);
    cameraDirection = glm::vec3(0, 0, 0);
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = normalize(cross(up, cameraDirection));
    cameraUp = glm::vec3(0, 1.0f, 0);
    cameraFront =  glm::vec3(0, 0, -1.0f);
    view = glm::lookAt(cameraPos, cameraTarget, up);
}

void DwarfCamera::RotateCameraWithTime(
    double time,
    const float radius,
    const vec3 direction) {

    float camX = sin(time) * radius;
    float camY = cos(time) * radius;
    view = lookAt(vec3(camX, 0, camY), vec3(0, 0, 0), direction);
}

void DwarfCamera::MoveCamera(glm::vec3 direction, vec3 _camDirection, float speed) {

    cameraPos += direction * speed;


    view = lookAt(cameraPos, cameraPos + _camDirection, cameraUp);
    shader->SetMatrix4("view", 1, GL_FALSE, view);
}



