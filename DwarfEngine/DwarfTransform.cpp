#include "DwarfTransform.h"

void Dwarf::Transform::DwarfTransform::RotateObject(
        glm::mat4 &trans, float radians, glm::vec3 axis) {
    trans = glm::rotate(trans, glm::radians(radians), axis);
}

void Dwarf::Transform::DwarfTransform::ScaleObject(
        glm::mat4 &trans, glm::vec3 scale) {
    trans = glm::scale(trans, scale);
}

void Dwarf::Transform::DwarfTransform::ApplyTransformChanges(glm::mat4 &trans) {
    shader->SetMatrix4("transform", 1, GL_FALSE, trans);
}



