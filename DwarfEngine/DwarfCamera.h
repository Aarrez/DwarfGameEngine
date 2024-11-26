#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "../DwarfGL/DwarfShader.h"

using namespace glm;

namespace Dwarf::Camera{


    class DwarfCamera{
    public:
        DwarfCamera();

        void RotateCameraWithTime(double time, const float radius);

    private:

        mat4 view {};

        vec3 cameraPos {};
        vec3 cameraTarget {};
        vec3 cameraDirection {};
        vec3 up {vec3(0, 1.0f, 0)};
        vec3 cameraRight {};
        vec3 cameraUp {};


    };

}
