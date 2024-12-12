#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui_stdlib.h>

#include "glm/gtx/matrix_decompose.hpp"



#pragma region Dwarf-Includes
//DwarfGL
/*#include "../DwarfGL/Mesh/DwarfMesh.h"*/
#include "../DwarfGL/Mesh/DwarfMesh2D.h"
#include "../DwarfGL/DwarfShader.h"
#include "../DwarfGL/Mesh/DwarfMesh.h"

//DwarfMisc
#include "../DwarfMisc/DwarfPath.h"
/*#include "../DwarfMisc/Memory.h"*/

//OBJLoaders
#include "../DwarfFileLoader/DwarfOBJLoader.h"
#include "../DwarfFileLoader/OptimizedOBJLoader.h"

//DwarfEngine
#include "DwarfEntityManager.h"
#include "DwarfCamera.h"
#include "DwarfInput.h"
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
        ImVec4 clear_color = ImVec4(.2, .2, .2, 1.0f);
        GLsizei Width = 800;
        GLsizei Height = 600;
        GLFWwindow* window {};
        std::shared_ptr<DwarfShader> shader {};

        std::unique_ptr<DwarfMesh2D> dwarfMesh2D {};
        std::unique_ptr<Camera::DwarfCamera> camera {};

        double deltaTime;

        //Imgui Variables

        bool show_demo_window {false};
        bool show_another_window {true};

        //Transform variables
        vec3 model_position {0, 0, 0};
        vec3 model_rotation {0, 0, 0};
        vec3 model_scale {.5f, .5f, .5f};
        float rad = 45.0f;
        int AmountOfMeshes {5};

    private:
        double currentTime;
        double lastTime;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
