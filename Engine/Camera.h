#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "../GL/Shader.h"

using namespace glm;

namespace Engine{


    class Camera{
    public:
        Camera(std::shared_ptr<Shader> shader);

        void MoveCamera(glm::vec3 direction, float speed);
        void SetCameraPos(const glm::vec3& pos);
        vec3 GetCameraPos();
        void SetCameraRotation(glm::vec3 rotation);
        glm::vec3 GetCameraRotation();
        void RotateCamera(double Xpos, double Ypos, bool first);


    private:

        //Rotate Camera stuff
        vec3 cameraDirection {};
        float lastX{}, lastY{}, offsetX{}, offsetY{};
        float sensitivity = 0.1f;
        float yaw{}, pitch{};
        //Other stuff
        mat4 view {};

        vec3 cameraPos {0, 0, 3.0f};
        vec3 cameraTarget {};

        vec3 up {vec3(0, 1.0f, 0)};
        vec3 cameraFront {};
        vec3 cameraRight {};
        vec3 cameraUp {};

        std::shared_ptr<Shader> shader {};

    };

}
#endif //CAMERA_H