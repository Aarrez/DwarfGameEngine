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
#include "../GL/VirtualObject.h"
#include "../GL/Shader.h"
#include "../GL/Mesh.h"

//DwarfMisc
#include "../Misc/Path.h"
/*#include "../DwarfMisc/Memory.h"*/


//OBJLoaders
#include "../FileLoader/OBJLoader.h"

//DwarfEngine
#include "EntityManager.h"
#include "Camera.h"
#include "Input.h"
#pragma endregion Dwarf-Includes


namespace Engine {

    class GameEngine {
    public:
        GameEngine();
        ~GameEngine();

        void Allocate();
        void InitializeGL();
        void Init();
        void Update();
        void Render();
        void Shutdown();

        //Variables
        ImVec4 clear_color = ImVec4(.2, .2, .2, 1.0f);
        GLsizei Width = 1200;
        GLsizei Height = 920;
        GLFWwindow* window {};

        std::shared_ptr<Shader> shader {};
        std::shared_ptr<Shader> lightShader {};

        std::unique_ptr<VirtualObject> virtual_object {};
        std::unique_ptr<Camera> camera {};

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
