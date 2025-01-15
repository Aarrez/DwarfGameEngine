#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace Engine{

    struct MovementValue {
        float horizontal, vertical, forward;

        bool operator==(const MovementValue & value) const {
            if (value.vertical == this->vertical &&
                value.horizontal == this->horizontal &&
                value.forward == this->forward) return true;

            return false;
        }

        bool operator!() const {
            if (this->forward != 0 ||
                this->horizontal != 0 ||
                this->vertical != 0) return true;
            return false;
        }

        [[nodiscard]] glm::vec3 ToVec3() const {
            return {this->forward, this->vertical, this->horizontal};
        }

        static MovementValue ValueZero() {
            return MovementValue{0.0f, 0.0f, 0.0f};
        }
    };

    /*
     * The instance pointer of this singleton is not deleted by this class.
     * I leave it up to the OS to free up the memory instead at the end of the program
     * If I for some ungodly reason decide to develop for mobile this might be a problem
     */
    class Input {



    public:
        static void Allocate(GLFWwindow* window);
        static Input& Get();
        static glm::vec3 GetMoveValue();
        static glm::vec3 GetCameraDirection();


    private:
        static Input* Instance;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void MoveKeys(const int &key, int &action);
        static glm::vec3 CamDirection, tempDirection;
        static glm::vec3 moveValue;
        static float lastX, lastY, offsetX, offsetY;
        static double lastMouseX, lastMouseY;
        static const float sensitivity;
        static float yaw, pitch;
        static bool firstMouse, RotateCamera;
        Input() = default;


    };
}
