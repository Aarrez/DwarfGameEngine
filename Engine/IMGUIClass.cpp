#include "IMGUIClass.h"

#include "../Managers/LightEntityManager.h"
#include "../Managers/TextureManager.h"

namespace Engine {

#pragma region Variable Declarations
    bool IMGUIClass::showDemoWindow = false;;

    string IMGUIClass::entity_buf {};
    string IMGUIClass::dest_preview_ent {};
    int IMGUIClass::dest_comb_select = 0;

    string IMGUIClass::model_buf {};
    int IMGUIClass::model_select_id = 0;
    string IMGUIClass::model_preview_ent {};
    int IMGUIClass::modelCombo_select_id = 0;

    int IMGUIClass::textures_select_id = 0;
    int IMGUIClass::textureCombo_select_id = 0;
    string IMGUIClass::texture_preview_ent {};
#pragma endregion

    void IMGUIClass::InitImGui(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        if (ImGui_ImplGlfw_InitForOpenGL(window, true) == false) {
            std::cerr << "Failed to Initialize IMGUI Implementation for GLFW" << std::endl;
        }
        if (ImGui_ImplOpenGL3_Init("#version 330") == false) {
            std::cerr << "Failed to initialize IMGUI Implementation for OpenGL3" << std::endl;
        }
    }

    void IMGUIClass::EndImGui() {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void IMGUIClass::InitUpdateLoop() {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void IMGUIClass::EndUpdateLoop() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void IMGUIClass::MenuBar() {
        if (ImGui::BeginMenu("File")) {
            ImGui::Checkbox("DemoWindow", &showDemoWindow);
            ImGui::EndMenu();
        }
    }

    void IMGUIClass::EntityWindow() {
        if (showDemoWindow) return;

         ImGui::Begin("EntityWindow");

            ImGui::InputText("Entity Name", &entity_buf);
            if (ImGui::Button("Create Entity")) {
                Texture tex {};
                tex.filePath = "Images/container.jpg";
                tex.colorFormat = GL_RGB;
                if (entity_buf.empty()) entity_buf = "Entity";
                EntityMessage msg(MessageType::CreateEntity, entity_buf);
                msg.texture = tex;
                msg.file = OBJLoader::FilesSerialized[0];
                EntityManager::ProcessMessages(msg);
            }
            dest_preview_ent = EntityManager::GetEntityList()[dest_comb_select]->name;
            if (ImGui::BeginCombo("EntitySelect", dest_preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::GetEntityList().size(); i++) {
                    const bool is_selected = dest_comb_select == i;
                    if (ImGui::Selectable(EntityManager::GetEntityList()[i]->name.c_str(), is_selected)) {
                        dest_comb_select = i;
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Destroy Entity")) {
                auto entName = EntityManager::GetEntityList()[dest_comb_select]->name;
                EntityManager::ProcessMessages(EntityMessage(MessageType::RemoveEntityByName, entName));
                if (dest_comb_select != 0) {
                    dest_comb_select += -1;
                }
            }

            for (auto & e : EntityManager::GetEntityList()) {
                if (ImGui::CollapsingHeader(e->name.c_str(), ImGuiTreeNodeFlags_None)) {
                    vec3 translation = e->GetPosition();
                    string posName = "Position##";
                    if (ImGui::DragFloat3(posName.c_str(), value_ptr(translation),
                        0.0f, -10.0f, 10.0f, "%.2f")) {
                        e->Translate(translation);
                        }
                    vec3 scale = e->GetScale();
                    string sclName = "Scale##";
                    if (ImGui::DragFloat3(sclName.c_str(), value_ptr(scale),
                        0.0f, 0.0f, 10.0f, "%.2f")) {
                        e->SetScale(scale);
                        }
                    vec3 rotation = e->GetRotation();
                    string sroName = "Rotation##";
                    if (ImGui::DragFloat3(sroName.c_str(), value_ptr(rotation),
                        0, -360, 360)) {
                        e->SetRotation(rotation);
                        }
                    e->SetTransformMatrix();
                }
            }
            ImGui::End();
    }

    void IMGUIClass::CameraWindow(Camera& camera) {
        if (showDemoWindow) return;
        if (ImGui::CollapsingHeader("Camera")) {
            vec3 camPos = camera.GetCameraPos();
            if (ImGui::DragFloat3("Camera Position", value_ptr(camPos),
                0.0f, -100.0f, 100.0f, "%.2f")) {
                camera.SetCameraPos(camPos);
                }

            vec3 camRot = camera.GetCameraRotation();
            if (ImGui::DragFloat3("Camera Rotation", value_ptr(camRot),
                0, -1, 1)) {
                camera.SetCameraRotation(camRot);
                }
        }
    }

    void IMGUIClass::ModelsWindow() {
        if (showDemoWindow) return;
        if (ImGui::Begin("ModelWindow")) {

            ImGui::InputText("Path to OBJ File", &model_buf);
            if (ImGui::Button("Serialize File")) {
                MeshMessage meshmsg(MessageType::AddMesh, model_buf);
                MeshManager::ProcessMessage(meshmsg);
            }
            if (ImGui::BeginListBox("Meshes")) {
                for (int i = 0; i < OBJLoader::FilesSerialized.size(); i++) {
                    const bool is_selected = model_select_id == i;
                    if (ImGui::Selectable(OBJLoader::FilesSerialized[i].fileName.c_str(), is_selected)) {
                        model_select_id = i;
                        model_preview_ent = OBJLoader::FilesSerialized[i].binPath;
                    }
                }
                ImGui::EndListBox();
            }

            model_preview_ent = EntityManager::GetEntityList()[modelCombo_select_id]->name;
            if (ImGui::BeginCombo("Entity Select", model_preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::GetEntityList().size(); i++) {
                    const bool is_selected = modelCombo_select_id == i;
                    if (ImGui::Selectable(EntityManager::GetEntityList()[i]->name.c_str(), is_selected)) {
                        modelCombo_select_id = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Change Entity Mesh")) {
                auto ent = EntityManager::GetEntityList()[modelCombo_select_id];
                const auto& meshFileInfo = OBJLoader::FilesSerialized.at(model_select_id);
                ent->meshName = MeshManager::Instance()->FindMesh(meshFileInfo.fileName).name;
            }
        }
        ImGui::End();
    }

    void IMGUIClass::TexturesWindow() {
        if (showDemoWindow) return;

        if (ImGui::Begin("Texture Window")) {
            if (ImGui::BeginListBox("Textures List")) {
                for(int i = 0; i < TextureManager::Instance()->GetTextures().size(); i++) {
                    const bool isSelected = i == textures_select_id;
                    if (ImGui::Selectable(
                        TextureManager::Instance()->GetTextures()[i].fileName.c_str(), isSelected)) {
                        textures_select_id = i;
                    }
                }
            }
            ImGui::EndListBox();
            texture_preview_ent = EntityManager::GetEntityList()[textureCombo_select_id]->name;
            if (ImGui::BeginCombo("Entity Select", texture_preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::GetEntityList().size(); i++) {
                    const bool is_selected = textureCombo_select_id == i;
                    if (ImGui::Selectable(EntityManager::GetEntityList()[i]->name.c_str(), is_selected)) {
                        textureCombo_select_id = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Change Texture")) {
                auto ent = EntityManager::GetEntityList()[textureCombo_select_id];
                ent->texture = TextureManager::Instance()->GetTextures()[textures_select_id];
            }
        }

        ImGui::End();
    }

    void IMGUIClass::LightsWindow() {
        if (showDemoWindow) return;
        if (ImGui::Begin("Lights Window")) {
            for (auto& lightEntity : LightEntityManager::Get().GetAllLights()) {
                ImGui::CollapsingHeader(
                    lightEntity->name.c_str());

                auto pos = lightEntity->GetPosition();
                if (ImGui::DragFloat3("Position##", value_ptr(pos),
                    0.0f, -100, 100)) {
                    lightEntity->SetPosition(pos);
                    }
                auto scale = lightEntity->GetScale();
                if (ImGui::DragFloat3("Scale##", value_ptr(scale),
                    0, -10, 10)) {
                    lightEntity->SetScale(scale);
                    }
            }

        }


        ImGui::End();
    }
}
