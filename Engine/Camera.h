#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "../GL/Shader.h"

using namespace glm;

namespace Engine{


    class Camera{
    public:
        Camera(std::shared_ptr<Shader> shader);

        void MoveCamera(glm::vec3 direction, vec3 camDirection, float speed);

        void RotateCameraWithTime(double time, float radius, vec3 direction = vec3(0, 1.0f, 0));

    private:

        mat4 view {};

        vec3 cameraPos {0, 0, 3.0f};
        vec3 cameraTarget {};
        vec3 cameraDirection {};
        vec3 up {vec3(0, 1.0f, 0)};
        vec3 cameraFront {};
        vec3 cameraRight {};
        vec3 cameraUp {};

        std::shared_ptr<Shader> shader {};

    };

}
