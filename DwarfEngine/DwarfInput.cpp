#include "DwarfInput.h"

#include <algorithm>

namespace Dwarf
{
    DwarfInput::DwarfInput(GLFWwindow *window) : window(window){
        glfwSetKeyCallback(window, key_callback);
    }

    void DwarfInput::input() {
    }

    void DwarfInput::key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
        /*switch (key) {
            case key == GLFW_KEY_ESCAPE && action == GLFW_PRESS:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            return;
            case key == GLFW_KEY_SPACE && action != GLFW_REPEAT:
                HandleMovementInput(
                    {movement_input.vertical, action, movement_input.forward});
            case key == GLFW_KEY_LEFT_CONTROL && action != GLFW_REPEAT:
                HandleMovementInput(
                    {movement_input.vertical, -action, movement_input.forward});
            case key == GLFW_KEY_W && action != GLFW_REPEAT:
                HandleMovementInput(
                    {movement_input.vertical, movement_input.horizontal, action});
            case key == GLFW_KEY_A && action != GLFW_REPEAT:
                HandleMovementInput(
                     {action, movement_input.horizontal, movement_input.forward});
            case key == GLFW_KEY_S && action != GLFW_REPEAT:
                HandleMovementInput(
                    {movement_input.vertical, movement_input.horizontal, -action});
            case key == GLFW_KEY_D && action != GLFW_REPEAT:
                HandleMovementInput(
                    {-action, movement_input.horizontal, movement_input.forward});
            default: ;
        }*/
    }

    void DwarfInput::HandleMovementInput(MovementValue value) {
        if (movement_input == value) return;

        if (movement_input.vertical != value.vertical)
            movement_input.vertical = 0;
        if (movement_input.horizontal != value.horizontal)
            movement_input.horizontal = 0;
        if (movement_input.forward != value.forward)
            movement_input.forward = 0;
    }

    MovementValue DwarfInput::GetMovementValue() {
        return movement_input;
    }

}






