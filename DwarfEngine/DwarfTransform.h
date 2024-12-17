#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../DwarfGL/DwarfShader.h"


namespace Dwarf::Transform{
    class DwarfTransform{
    public:
        DwarfTransform(DwarfShader* dwarf_shader)
        : shader(dwarf_shader){}

        void RotateObject(glm::mat4 &trans, float radians, glm::vec3 axis);
        void ScaleObject(glm::mat4 &trans, glm::vec3 scale);
        void ApplyTransformChanges(glm::mat4 &trans);

    private:
        DwarfShader* shader;
    };
}