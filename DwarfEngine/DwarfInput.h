#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <string>
#include <glm/vec3.hpp>

namespace Dwarf{
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
    };

    /*
     * The instance pointer of this singleton is not deleted by this class.
     * I leave it up to the OS to free up the memory instead at the end of the program
     * If I for some ungodly reason decide to develop for mobile this might be a problem
     */
    class DwarfInput {

        DwarfInput() = default;

    public:
        static void Allocate(GLFWwindow* window);
        static DwarfInput& Get();
        static MovementValue* GetMoveValue();

    private:
        static DwarfInput* Instance;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    };
}
