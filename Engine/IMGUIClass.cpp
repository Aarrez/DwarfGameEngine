#include "IMGUIClass.h"



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
    int IMGUIClass::spec_texture_select_id = 0;
    int IMGUIClass::textureCombo_select_id = 0;
    string IMGUIClass::texture_preview_ent {};

    LightTypes IMGUIClass::combo_type {LightTypes::DirectionalLight};

    glm::vec3 IMGUIClass::light_ambient {.2f};
    float IMGUIClass::light_ambient_intensity {1.0f};
    glm::vec3 IMGUIClass::light_diffuse {.5f};
    float IMGUIClass::light_diffuse_intensity {1.0f};
    glm::vec3 IMGUIClass::light_specular {1.0f};
    float IMGUIClass::light_specular_intensity {1.0f};

    glm::vec3 IMGUIClass::mat_ambient {1.0f, 0.5f, 0.31f};
    float IMGUIClass::mat_ambient_intensity {1.0f};
    glm::vec3 IMGUIClass::mat_diffuse {1.0f, 0.5f, 0.31};
    float IMGUIClass::mat_diffuse_intensity {1.0f};
    glm::vec3 IMGUIClass::mat_specular {0.5f};
    float IMGUIClass::mat_specular_intensity {1.0f};


    vector<float> IMGUIClass::shininess_list  {2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f, 256.0f};
    int IMGUIClass::light_Select_Id = 0;
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
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::Checkbox("DemoWindow", &showDemoWindow);
                ImGui::Checkbox("Simulate", &run_simulation);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }


    }

    void IMGUIClass::EntityWindow(Shader& shader) {
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
                EntityManager::Get().ProcessMessages(msg);
            }
            dest_preview_ent = EntityManager::Get().GetEntityList()[dest_comb_select]->name;
            if (ImGui::BeginCombo("EntitySelect", dest_preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::Get().GetEntityList().size(); i++) {
                    const bool is_selected = dest_comb_select == i;
                    if (ImGui::Selectable(EntityManager::Get().GetEntityList()[i]->name.c_str(), is_selected)) {
                        dest_comb_select = i;
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Destroy Entity")) {
                auto entName = EntityManager::Get().GetEntityList()[dest_comb_select]->name;
                EntityManager::Get().ProcessMessages(EntityMessage(MessageType::RemoveEntityByName, entName));
                if (dest_comb_select != 0) {
                    dest_comb_select += -1;
                }
            }

            for (auto & e : EntityManager::Get().GetEntityList()) {
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

                    if (ImGui::BeginCombo("Shininess", std::to_string(e->shininess).c_str())) {
                        for (int i = 0; i < shininess_list.size(); i++) {
                            bool selected = light_Select_Id == i;
                            if (ImGui::Selectable(std::to_string(shininess_list[i]).c_str(), selected)) {
                                light_Select_Id = i;
                                e->shininess = shininess_list[i];
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
                shader.SetFloat("material.shininess", e->shininess);
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

            model_preview_ent = EntityManager::Get().GetEntityList()[modelCombo_select_id]->name;
            if (ImGui::BeginCombo("Entity Select", model_preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::Get().GetEntityList().size(); i++) {
                    const bool is_selected = modelCombo_select_id == i;
                    if (ImGui::Selectable(EntityManager::Get().GetEntityList()[i]->name.c_str(), is_selected)) {
                        modelCombo_select_id = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Change Entity Mesh")) {
                auto ent = EntityManager::Get().GetEntityList()[modelCombo_select_id];
                const auto& meshFileInfo = OBJLoader::FilesSerialized.at(model_select_id);
                ent->meshName = MeshManager::Instance()->FindMesh(meshFileInfo.fileName).name;
            }
        }
        ImGui::End();
    }

    void IMGUIClass::TexturesWindow() {
        if (showDemoWindow) return;

        if (ImGui::Begin("Texture Window")) {
            size_t size = TextureManager::Get()->GetTextures().size();
            if (ImGui::BeginListBox("Textures List", {150, size * 20.0f})) {
                for(int i = 0; i < size; i++) {
                    const bool isSelected = i == textures_select_id;
                    if (ImGui::Selectable(
                        TextureManager::Get()->GetTextures()[i].fileName.c_str(), isSelected)) {
                        textures_select_id = i;
                    }
                }
            }
            ImGui::EndListBox();

            texture_preview_ent = EntityManager::Get().GetEntityList()[textureCombo_select_id]->name;
            if (ImGui::BeginCombo("Entity Select", texture_preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::Get().GetEntityList().size(); i++) {
                    const bool is_selected = textureCombo_select_id == i;
                    if (ImGui::Selectable(EntityManager::Get().GetEntityList()[i]->name.c_str(), is_selected)) {
                        textureCombo_select_id = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Change Texture")) {
                auto ent = EntityManager::Get().GetEntityList()[textureCombo_select_id];
                ent->texture = TextureManager::Get()->GetTextures()[textures_select_id];
            }


            if (ImGui::BeginListBox("Specular Texture List", {150, size * 20.0f})) {
                for(int i = 0; i < size; i++) {
                    const bool isSelected = i == spec_texture_select_id;
                    if (ImGui::Selectable(
                        TextureManager::Get()->GetTextures()[i].fileName.c_str(), isSelected)) {
                        spec_texture_select_id = i;
                        }
                }
            }
            ImGui::EndListBox();

            if (ImGui::Button("Change Specular Texture")) {
                auto ent = EntityManager::Get().GetEntityList()[textureCombo_select_id];
                ent->spec_texture = TextureManager::Get()->GetTextures()[spec_texture_select_id];
            }
        }

        ImGui::End();
    }

    void IMGUIClass::LightsWindow(Shader& shader) {
        if (showDemoWindow) return;
        if (ImGui::Begin("Lights Window")) {
            for (auto& lightEntity : LightEntityManager::Get().GetAllLights()) {
                std::string name = lightEntity->name;
                if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                    auto pos = lightEntity->GetPosition();
                    if (ImGui::DragFloat3("Position##", value_ptr(pos),
                        0.0f, -10, 10)) {
                        lightEntity->SetPosition(pos);
                        }
                    auto scale = lightEntity->GetScale();
                    if (ImGui::DragFloat3("Scale##", value_ptr(scale),
                        0, -10, 10)) {
                        lightEntity->SetScale(scale);
                        }
                    if (ImGui::BeginCombo("LightType", ToString(combo_type))) {
                        bool point_select = combo_type == LightTypes::PointLight;
                        if (ImGui::Selectable(ToString(LightTypes::PointLight), point_select)) {
                            combo_type = LightTypes::PointLight;
                        }
                        bool spot_select = combo_type == LightTypes::SpotLight;
                        if (ImGui::Selectable(ToString(LightTypes::SpotLight), spot_select)) {
                            combo_type = LightTypes::SpotLight;
                        }
                        bool dir_select = combo_type == LightTypes::DirectionalLight;
                        if (ImGui::Selectable(ToString(LightTypes::DirectionalLight), dir_select)) {
                            combo_type = LightTypes::DirectionalLight;
                        }
                        ImGui::EndCombo();
                    }
                    lightEntity->ChangeLightType(combo_type);
                    glm::vec3 dir;
                    switch (combo_type) {
                        case LightTypes::PointLight:
                            break;
                        case LightTypes::SpotLight:
                            dir = lightEntity->GetLightDirection();
                            ImGui::DragFloat3("Light Direction##",
                                value_ptr(dir), 0.1f, -100, 100);
                            break;

                        case LightTypes::DirectionalLight:
                            dir = lightEntity->GetLightDirection();
                            if (ImGui::DragFloat3("Light Direction##",
                                value_ptr(dir), 0.1f, -100 , 100)) {
                                lightEntity->SetLightDirection(dir, shader);
                            }
                            break;
                    }

                    ImGui::Dummy({0, 10});
                    ImGui::Text("Light values:");

                    ImGui::ColorEdit3("Ambient##", value_ptr(light_ambient));
                    ImGui::DragFloat("AmbientIntensity##", &light_ambient_intensity, 0.1, 1, 5);
                    auto ambient_result = light_ambient_intensity * light_ambient;
                    lightEntity->SetAmbient(shader, ambient_result);

                    ImGui::ColorEdit3("Diffuse##", value_ptr(light_diffuse));
                    ImGui::DragFloat("DiffuseIntensity##", &light_diffuse_intensity, .1, 1, 5);
                    auto diffuse_result = light_diffuse * light_diffuse_intensity;
                    lightEntity->SetDiffuse(shader, diffuse_result);

                    ImGui::ColorEdit3("Specular##", value_ptr(light_specular));
                    ImGui::DragFloat("SpecularIntensity##", &light_specular_intensity, .1, 1, 5);
                    auto specular_result = light_specular * light_specular_intensity;
                    lightEntity->SetSpecular(shader, specular_result);
                }
                lightEntity->CombineModels();
            }
        }
        ImGui::End();
    }
}
