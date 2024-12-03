#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "../DwarfGL/DwarfShader.h"

using namespace glm;

namespace Dwarf::Camera{


    class DwarfCamera{
    public:
        DwarfCamera(std::shared_ptr<DwarfShader> shader);

        void MoveCamera(glm::vec3 direction, float speed);
        void RotateCamera(glm::vec3 axis, float angle);

        void RotateCameraWithTime(double time, float radius, vec3 direction = vec3(0, 1.0f, 0));

    private:

        mat4 view {};

        vec3 cameraPos {};
        vec3 cameraTarget {};
        vec3 cameraDirection {};
        vec3 up {vec3(0, 1.0f, 0)};
        vec3 cameraFront {};
        vec3 cameraRight {};
        vec3 cameraUp {};

        std::shared_ptr<DwarfShader> shader {};

    };

}
