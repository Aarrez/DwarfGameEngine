#ifndef GameEngine_h
#define GameEngine_h
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

#pragma region Dwarf-Includes
//Singelton Managers
#include "../Managers/ShadowManager.h"
#include "../Managers/ThreadManager.h"
#include "../Managers/MeshManager.h"
#include "../Managers/EntityManager.h"
#include "../Managers/TextureManager.h"

#include "../GL/VirtualObject.h"
#include "../GL/Shader.h"

//DwarfMisc
#include "../Misc/Path.h"
#include "../Misc/Memory.h"

#include "../Physics/Physics.h"

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
        void Shutdown();

        void RenderScene(Shader& shader);
        /*void TempRenderScene(Shader& shader);*/
        void RenderNormalScene(Shader& shader, glm::mat4& lightSpaceMatrix);
        //Variables
        ImVec4 clear_color = ImVec4(.1, .1, .1, 1.0f);
        GLsizei Width = 1200;
        GLsizei Height = 920;
        GLFWwindow* window {};

        std::shared_ptr<Shader> lightShader {};
        std::shared_ptr<Shader> simpleDepthShader {};
        std::shared_ptr<Shader> debugDepthMapQuad {};

        std::unique_ptr<VirtualObject> virtual_object {};
        std::unique_ptr<Camera> camera {};

        Mesh cube_mesh;

        double deltaTime;

        glm::vec3 lightPos = {-2.0f, 3.0f, 1.0f};

        std::vector<std::string> textures;

    private:

        float light_near_plane = .1f;
        float light_far_plane = 10.0f;

        float near_plane = 0.1f;
        float far_plane = 100.0f;

        double lastTime, nowTime;
        double limitFPS = 1.0/60.0;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    };

} // Dwarf
#endif //GameEngine_h