#include "DwarfCamera.h"

using namespace Dwarf::Camera;

DwarfCamera::DwarfCamera() {
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), 800.0f/ 600.0f, 0.1f, 100.0f);
}

void DwarfCamera::RotateModel(double time, float radians, glm::vec3 direction) {
    model = glm::rotate(model, (float)time * glm::radians(radians), direction);
}
