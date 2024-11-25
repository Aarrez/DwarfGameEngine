#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace Dwarf{
    class DwarfInput {

    public:
        DwarfInput(GLFWwindow* window);
        ~DwarfInput();

        void glfwSetKeyCallback(GLFWwindow * window, void(DwarfInput::* callback)(GLFWwindow *window, int key, int scancode, int action, int mods));

        void input();

        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    private:
        GLFWwindow* window{};

    };
}
