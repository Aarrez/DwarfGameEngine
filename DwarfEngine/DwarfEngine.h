#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../DwarfGL/DwarfShader.h"


namespace Dwarf {

    class DwarfEngine {
    public:
        explicit DwarfEngine();
        ~DwarfEngine();

        void Init();
        void Update();
        void Render();
        void Shutdown();

        //Variables
        GLsizei Width, Height;\
        GLFWwindow* window {};
        DwarfShader* shader {};

    private:

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
