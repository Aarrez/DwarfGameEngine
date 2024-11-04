#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "DwarfEngine/DwarfEngine.h"


int main() {
    GLFWwindow* window {};
    Dwarf::DwarfEngine engine(window);
    engine.Init();
    while(!glfwWindowShouldClose(window)) {
        engine.Update();
        engine.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    engine.Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
