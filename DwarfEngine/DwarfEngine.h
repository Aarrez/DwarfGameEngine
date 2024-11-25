#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



#pragma region Dwarf-Includes
//DwarfGL
/*#include "../DwarfGL/Mesh/DwarfMesh.h"*/
#include "../DwarfGL/Mesh/DwarfMesh2D.h"
#include "../DwarfGL/DwarfShader.h"
#include "../DwarfGL/Mesh/DwarfMesh.h"

//DwarfMisc
#include "../DwarfMisc/DwarfPath.h"

//DwarfEngine
#include "DwarfEntityManager.h"
#include "DwarfCamera.h"
#include "DwarfTransfrom.h"
#pragma endregion Dwarf-Includes


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
        ImVec4 clear_color = ImVec4(0.0f/255.f, 0.0f/255.f, 0.0f/255.f, 1.0f);
        GLsizei Width = 800;
        GLsizei Height = 600;
        GLFWwindow* window {};
        DwarfShader* shader {};
        Mesh::DwarfModel* dwarfModel {};
        Mesh2D::DwarfMesh2D* dwarfMesh2D {};
        Entity::DwarfEntityManager* DEM {};

        double deltaTime;

        //Imgui Variables

        bool show_demo_window {true};
        bool show_another_window {false};

        //Transform variables
        Camera::DwarfCamera* camera;

        //Temp Variables
        vector<unique_ptr<Entity::Entity>> entityList;


    private:
        double currentTime;
        double lastTime;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
