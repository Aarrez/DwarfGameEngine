#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "../DwarfGL/DwarfShader.h"


namespace Dwarf::Camera{


    class DwarfCamera{
    public:
        DwarfCamera();

        void RotateModelWithTime(double time, float radians, glm::vec3 direction);
        void SetModelRotation(float radians, glm::vec3 direction);

        glm::mat4 model {};
        glm::mat4 view {};
        glm::mat4 projection {};

    };

}
