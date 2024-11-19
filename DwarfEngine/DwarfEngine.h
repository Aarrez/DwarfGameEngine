#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../DwarfGL/DwarfShader.h"
#include "../DwarfGL/Mesh/DwarfMesh.h"
#include "../DwarfMisc/DwarfPath.h"
#include "DwarfCamera.h"
#include "DwarfTransfrom.h"
#include <memory>
#include "../DwarfGL/Mesh/DwarfMesh.h"



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
        GLsizei Width = 800;
        GLsizei Height = 600;
        GLFWwindow* window {};
        DwarfShader* shader {};
        Mesh::DwarfModel* dwarfModel {};
        Transform::DwarfTransform* dwarfTransform {};

        //Transform variables
        glm::mat4 transform;
        Camera::DwarfCamera* camera;


    private:

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
