#ifndef GameEngine_h
#define GameEngine_h
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include "glm/gtx/matrix_decompose.hpp"

#pragma region Dwarf-Includes
//Singelton Managers
#include "../Managers/ShadowManager.h"
#include "../Managers/ThreadManager.h"
#include "../Managers/MeshManager.h"
#include "../Managers/EntityManager.h"
#include "../Managers/TextureManager.h"


/*#include "../DwarfGL/Mesh/DwarfMesh.h"*/
#include "../GL/VirtualObject.h"
#include "../GL/Shader.h"
#include "../GL/Mesh.h"

//DwarfMisc
#include "../Misc/Path.h"
#include "../Misc/Memory.h"
/*#include "../DwarfMisc/Memory.h"*/

//OBJLoaders
#include "../FileLoader/OBJLoader.h"
#include "Camera.h"
#include "Input.h"
#include "IMGUIClass.h"



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
        void RenderScene(Shader& shader);
        void Shutdown();

        //Variables
        ImVec4 clear_color = ImVec4(.2, .2, .2, 1.0f);
        GLsizei Width = 1200;
        GLsizei Height = 920;
        GLFWwindow* window {};

        std::shared_ptr<Shader> mainShader {};
        std::shared_ptr<Shader> lightShader {};
        std::shared_ptr<Shader> simpleDepthShader {};
        std::shared_ptr<Shader> debugDepthMapQuad {};

        std::unique_ptr<VirtualObject> virtual_object {};
        std::unique_ptr<Camera> camera {};

        Mesh cube_mesh;
        ShadowMap shadow_map;

        double deltaTime;

        //Transform variables
        glm::vec3 model_position {0, 0, 0};
        glm::vec3 model_rotation {0, 0, 0};
        glm::vec3 model_scale {.5f, .5f, .5f};
        float rad = 45.0f;
        int AmountOfMeshes {5};

        std::vector<std::string> textures;

    private:
        double currentTime;
        double lastTime;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
#endif //GameEngine_h