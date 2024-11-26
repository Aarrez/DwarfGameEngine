#include "DwarfCamera.h"

#include <glad/glad.h>

using namespace Dwarf::Camera;

DwarfCamera::DwarfCamera() {
    view = mat4(1.0f);
    cameraPos = glm::vec3(0,0,3.0f);
    cameraDirection = glm::vec3(0, 0, 0);
    cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);
    view = glm::lookAt(cameraPos, cameraTarget, up);
}

void DwarfCamera::RotateCameraWithTime(double time, const float radius) {

}

