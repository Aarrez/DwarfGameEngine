//
// Created by Aaron on 2024-11-04.
//

#include "DwarfInput.h"

Dwarf::DwarfInput::DwarfInput(GLFWwindow *window) {
    this->window = window;
    glfwSetKeyCallback(window, &DwarfInput::key_callback);
}

void Dwarf::DwarfInput::input() {

}

void Dwarf::DwarfInput::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

Dwarf::DwarfInput::~DwarfInput() {

}

void Dwarf::DwarfInput::glfwSetKeyCallback(GLFWwindow *window,
    void(DwarfInput::*callback)(GLFWwindow *window, int key, int scancode, int action, int mods)) {
}




