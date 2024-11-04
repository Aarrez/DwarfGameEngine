#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



namespace Dwarf {

    class DwarfEngine {
    public:
        explicit DwarfEngine(GLFWwindow* window);
        ~DwarfEngine();

        void Init();
        void Update();
        void Render();
        void Shutdown();

        //Variables
        GLsizei Width, Height;

    private:
        GLFWwindow* window;
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
