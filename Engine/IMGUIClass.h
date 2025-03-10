#ifndef IMGUICLASS_H
#define IMGUICLASS_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui_stdlib.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../Managers/LightEntityManager.h"
#include "../Managers/TextureManager.h"

#include "Camera.h"
#include "../Misc/PublicStructs.h"
#include "../Message/EntityMessage.h"
#include "../FileLoader/OBJLoader.h"
#include "../Managers/EntityManager.h"
#include "../Message/MeshMessage.h"
#include "../Managers/MeshManager.h"
#include "../Misc/LightVariables.h"

namespace Engine {
    class IMGUIClass {
        IMGUIClass() = default;
    public:
        //Inizialize ImGui and create a context
        static void InitImGui(GLFWwindow* window);
        static void EndImGui();

        //Initalize New Frame
        static void InitUpdateLoop();
        static void EndUpdateLoop();

        //Start of UI windows
        static void MenuBar();

        static void EntityWindow(Shader& shader);
        static void CameraWindow(Camera& camera);
        static void ModelsWindow();
        static void TexturesWindow();
        static void LightsWindow(Shader& shader);
        static void PhysicsWindow();

    private:
        static bool showDemoWindow;

        static bool run_simulation;

        static std::string entity_buf;
        static std::string dest_preview_ent;
        static int dest_comb_select;

        static std::string model_buf;
        static int model_select_id;
        static std::string model_preview_ent;
        static int modelCombo_select_id;

        static int textures_select_id;
        static int spec_texture_select_id;
        static int textureCombo_select_id;

        static LightTypes combo_type;
        //Light creating variables
        static LightTypes create_combo_type;

        //Material values
        static glm::vec3 mat_ambient;
        static float mat_ambient_intensity;
        static glm::vec3 mat_diffuse;
        static float mat_diffuse_intensity;
        static glm::vec3 mat_specular;
        static float mat_specular_intensity;

        static std::vector<float> shininess_list;
        static int light_Select_Id;

        static std::string destroy_name_preview;
        static int destroy_select_id;

        //Physics
        static glm::vec3 force;
    };
}


#endif //IMGUICLASS_H
