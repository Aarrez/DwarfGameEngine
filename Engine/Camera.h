#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "../GL/Shader.h"

namespace Engine{


    class Camera{
    public:
        Camera();

        void MoveCamera(glm::vec3 direction, float speed, glm::mat4* view);
        void SetCameraPos(const glm::vec3& pos);
        glm::vec3 GetCameraPos();
        void SetCameraRotation(glm::vec3 rotation);
        glm::vec3 GetCameraRotation();
        void RotateCamera(double Xpos, double Ypos, bool first);

    private:

        //Rotate Camera stuff
        glm::vec3 cameraDirection {};
        float lastX{}, lastY{}, offsetX{}, offsetY{};
        float sensitivity = 0.1f;
        float yaw{}, pitch{};

        //Other stuff
        glm::mat4 view {};

        glm::vec3 cameraPos {0, 0, 3.0f};
        glm::vec3 cameraTarget {};

        glm::vec3 up {glm::vec3(0, 1.0f, 0)};
        glm::vec3 cameraFront {};
        glm::vec3 cameraRight {};
        glm::vec3 cameraUp {};

    };

}
#endif //CAMERA_H