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

#include "Camera.h"
#include "../Misc/PublicStructs.h"
#include "../Message/EntityMessage.h"
#include "../FileLoader/OBJLoader.h"
#include "../Managers/EntityManager.h"
#include "../Message/MeshMessage.h"
#include "../Managers/MeshManager.h"

using namespace glm;

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

        static void EntityWindow();
        static void CameraWindow(Camera& camera);
        static void ModelsWindow();
        static void TexturesWindow();
        static void LightsWindow();

    private:
        static bool showDemoWindow;

        static string entity_buf;
        static string dest_preview_ent;
        static int dest_comb_select;

        static string model_buf;
        static int model_select_id;
        static string model_preview_ent;
        static int modelCombo_select_id;

        static int textures_select_id;
        static int textureCombo_select_id;
        static string texture_preview_ent;

    };
}


#endif //IMGUICLASS_H
