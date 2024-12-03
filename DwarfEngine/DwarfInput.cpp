#include "DwarfInput.h"

#include <algorithm>
#include <iostream>

namespace Dwarf
{
    DwarfInput* DwarfInput::Instance = nullptr;

    MovementValue moveValue {0, 0, 0};

    void DwarfInput::Allocate(GLFWwindow *window) {
        assert(Instance == nullptr);
        if (Instance) return;
        Instance = new DwarfInput();
        glfwSetKeyCallback(window, key_callback);
    }

    DwarfInput & DwarfInput::Get() {
        if (!Instance)
            Allocate(nullptr);
        return *Instance;
    }

    MovementValue* DwarfInput::GetMoveValue() {
        return &moveValue;
    }

    void DwarfInput::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (action == GLFW_REPEAT) return;
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            return;
            case GLFW_KEY_SPACE:
                if (moveValue.vertical == -1) action = 0;
                moveValue = MovementValue(moveValue.horizontal, action, moveValue.forward);
            break;

            case GLFW_KEY_LEFT_CONTROL:
                if (moveValue.vertical == 1) action = 0;
                moveValue = MovementValue(moveValue.horizontal, -action, moveValue.forward);
            break;

            case GLFW_KEY_W: case GLFW_KEY_UP:
                if (moveValue.forward == -1) action = 0;
                moveValue = MovementValue(moveValue.horizontal, moveValue.vertical, action);
            break;

            case GLFW_KEY_DOWN: case GLFW_KEY_S:
                if (moveValue.forward == 1) action = 0;
                moveValue = MovementValue(moveValue.horizontal, moveValue.vertical, -action);
            break;

            case GLFW_KEY_RIGHT: case GLFW_KEY_D:
                if (moveValue.horizontal == -1) action = 0;
                moveValue = MovementValue(action, moveValue.vertical, moveValue.forward);
                auto some = moveValue;
            break;

            case GLFW_KEY_LEFT: case GLFW_KEY_A:
                if (moveValue.horizontal == 1) action = 0;
                moveValue = MovementValue(-action, moveValue.vertical, moveValue.forward);
            break;

            default:
                moveValue = {0, 0, 0};
        }
    }
}






