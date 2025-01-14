#include "DwarfInput.h"

#include <algorithm>
#include <iostream>
#include <glm/trigonometric.hpp>
#include <glm/detail/func_geometric.inl>

namespace Engine
{
    DwarfInput* DwarfInput::Instance = nullptr;
    const float DwarfInput::sensitivity = 0.1f;
    float DwarfInput::lastX, DwarfInput::lastY, DwarfInput::yaw, DwarfInput::pitch = 0.0f;
    float DwarfInput::offsetX, DwarfInput::offsetY = 0.0f;
    double DwarfInput::lastMouseX, DwarfInput::lastMouseY = 0.0;
    bool DwarfInput::firstMouse = true;
    bool DwarfInput::RotateCamera = false;
    glm::vec3 DwarfInput::moveValue, DwarfInput::CamDirection, DwarfInput::tempDirection {0, 0, 0};



    void DwarfInput::Allocate(GLFWwindow *window) {
        assert(Instance == nullptr);
        if (Instance) return;
        Instance = new DwarfInput();
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        lastX = width / 2.0f;
        lastY = height / 2.0f;
    }

    DwarfInput & DwarfInput::Get() {
        if (!Instance)
            Allocate(nullptr);
        return *Instance;
    }

    glm::vec3 DwarfInput::GetMoveValue() {
        return moveValue;
    }

    glm::vec3 DwarfInput::GetCameraDirection() {
        return CamDirection;
    }

    void DwarfInput::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_REPEAT) return;

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        MoveKeys(key, action);
    }

    void DwarfInput::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
        if (!RotateCamera) return;
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        offsetX = xpos - lastX;
        offsetY = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        offsetX *= sensitivity;
        offsetY *= sensitivity;
        yaw += offsetX;
        pitch += offsetY;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        tempDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        tempDirection.y = sin(glm::radians(pitch));
        tempDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        CamDirection = glm::normalize(tempDirection);
    }

    void DwarfInput::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
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

    void DwarfInput::MoveKeys(const int &key, int &action) {
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






