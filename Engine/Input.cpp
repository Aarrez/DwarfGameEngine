#include "Input.h"

namespace Engine
{
    Input* Input::Instance = nullptr;
    Camera* Input::m_camera = nullptr;
    const float Input::sensitivity = 0.1f;
    float Input::lastX, Input::lastY, Input::yaw, Input::pitch = 0.0f;
    float Input::offsetX, Input::offsetY = 0.0f;
    double Input::lastMouseX, Input::lastMouseY = 0.0;
    bool Input::firstMouse = true;
    bool Input::RotateCamera = false;
    glm::vec3 Input::moveValue, Input::CamDirection, Input::tempDirection {0, 0, 0};



    void Input::Allocate(GLFWwindow *window) {
        assert(Instance == nullptr);
        if (Instance) return;
        Instance = new Input();
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        lastX = width / 2.0f;
        lastY = height / 2.0f;
    }

    void Input::SetCameraRef(Camera *camera) {
        m_camera = camera;
    }


    Input & Input::Get() {
        if (!Instance)
            Allocate(nullptr);
        return *Instance;
    }

    glm::vec3 Input::GetMoveValue() {
        return moveValue;
    }

    glm::vec3 Input::GetCameraDirection() {
        return CamDirection;
    }

    void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_REPEAT) return;

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        MoveKeys(key, action);
    }

    void Input::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
        if (!RotateCamera) return;
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            m_camera->RotateCamera(xpos, ypos, firstMouse);
            firstMouse = false;
            return;
        }
        m_camera->RotateCamera(xpos, ypos, firstMouse);
    }

    void Input::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action != GLFW_REPEAT) {
            if (action == GLFW_PRESS) {
                RotateCamera = true;
                glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
                glfwSetCursorPos(window,lastX, lastY);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }

            if (action == GLFW_RELEASE) {
                RotateCamera = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPos(window,lastMouseX, lastMouseY);
            }

        }
    }

    void Input::MoveKeys(const int &key, int &action) {
        if (key == GLFW_KEY_SPACE || key == GLFW_KEY_E) {
            if (moveValue.y == -1) action = 0;
            moveValue = glm::vec3(moveValue.x, action, moveValue.z);
        }
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_Q) {
            if (moveValue.y == 1) action = 0;
            moveValue = glm::vec3(moveValue.x, -action, moveValue.z);
        }
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
            if (moveValue.x == 1) action = 0;
            moveValue = glm::vec3(moveValue.x, moveValue.y, -action);
        }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            if (moveValue.x == -1) action = 0;
            moveValue = glm::vec3(moveValue.x, moveValue.y, action);
        }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            if (moveValue.z == 1) action = 0;
            moveValue = glm::vec3(-action, moveValue.y, moveValue.z);
        }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            if (moveValue.z == -1) action = 0;
            moveValue = glm::vec3(action, moveValue.y, moveValue.z);
        }
    }
}






