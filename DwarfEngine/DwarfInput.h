#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace Dwarf{

    struct MovementValue {
        int vertical, horizontal, forward;

        bool operator==(const MovementValue & value) const {
            if (value.vertical == this->vertical &&
                value.horizontal == this->horizontal &&
                value.forward == this->forward) return true;

            return false;
        }
    };

    class DwarfInput {

    public:
        explicit DwarfInput(GLFWwindow* window);

        void input();

        MovementValue GetMovementValue();


    private:

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        GLFWwindow* window{};

        static MovementValue movement_input;

        static void HandleMovementInput(MovementValue value);
    };
}
