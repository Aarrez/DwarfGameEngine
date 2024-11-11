#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../DwarfGL/DwarfShader.h"
#include "../DwarfGL/Mesh/DwarfMesh.h"


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
        GLsizei Width, Height;
        GLFWwindow* window {};
        DwarfShader* shader {};
        Mesh::DwarfMesh2D* mesh_2d {};
        Mesh::DwarfMesh* mesh {};


    private:

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
