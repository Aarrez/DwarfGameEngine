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
#include <thread>
#include <mutex>

#include "glm/gtx/matrix_decompose.hpp"



#pragma region Dwarf-Includes
//DwarfGL
#include "MeshManager.h"

/*#include "../DwarfGL/Mesh/DwarfMesh.h"*/
#include "../DwarfGL/Mesh/VirtualObject.h"
#include "../DwarfGL/DwarfShader.h"
#include "../DwarfGL/Mesh/Mesh.h"

//DwarfMisc
#include "../DwarfMisc/DwarfPath.h"
/*#include "../DwarfMisc/Memory.h"*/


//OBJLoaders
#include "../DwarfFileLoader/OBJLoader.h"

//DwarfEngine
#include "DwarfEntityManager.h"
#include "DwarfCamera.h"
#include "DwarfInput.h"
#pragma endregion Dwarf-Includes


namespace Engine {

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
        GLsizei Width = 1200;
        GLsizei Height = 920;
        GLFWwindow* window {};

        std::shared_ptr<DwarfShader> shader {};
        std::shared_ptr<DwarfShader> lightShader {};

        std::unique_ptr<VirtualObject> dwarfMesh2D {};
        std::unique_ptr<Camera::DwarfCamera> camera {};

        double deltaTime;

        //ImGui ListBox
        int selected_int;
        string selected_path;

        string file_buf, ent_buf;

        //ImGui Combobox
        string preview_ent;
        int comb_selected = 0;


        bool show_demo_window {false};
        bool show_another_window {true};

        //Transform variables
        vec3 model_position {0, 0, 0};
        vec3 model_rotation {0, 0, 0};
        vec3 model_scale {.5f, .5f, .5f};
        float rad = 45.0f;
        int AmountOfMeshes {5};

        vector<string> textures;

    private:
        double currentTime;
        double lastTime;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
