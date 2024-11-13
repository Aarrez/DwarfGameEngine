#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../DwarfGL/DwarfShader.h"
#include "../DwarfGL/Mesh/DwarfMesh.h"
#include "../DwarfMisc/DwarfPath.h"



namespace Dwarf {

    class DwarfEngine {
    public:
        DwarfEngine();
        ~DwarfEngine();

        void Init();
        void Update();
        void Render();
        void Shutdown();

        //Variables
        GLsizei Width, Height;
        GLFWwindow* window {};
        DwarfShader* shader {};
        Mesh2D::DwarfMesh2D* mesh2D {};


    private:

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
