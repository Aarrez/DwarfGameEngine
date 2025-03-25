#include "IMGUIClass.h"



namespace Engine {

#pragma region Variable Declarations
    bool IMGUIClass::showDemoWindow = false;
    bool IMGUIClass::run_simulation = false;

    std::string IMGUIClass::entity_buf {};
    std::string IMGUIClass::dest_preview_ent {};
    int IMGUIClass::dest_comb_select = 0;

    std::string IMGUIClass::model_buf {};
    int IMGUIClass::model_select_id = 0;
    std::string IMGUIClass::model_preview_ent {};
    int IMGUIClass::modelCombo_select_id = 0;

    int IMGUIClass::textures_select_id = 0;
    int IMGUIClass::spec_texture_select_id = 0;
    int IMGUIClass::textureCombo_select_id = 0;

    LightTypes IMGUIClass::combo_type {LightTypes::DirectionalLight};
    LightTypes IMGUIClass::create_combo_type {LightTypes::DirectionalLight};

    glm::vec3 IMGUIClass::mat_ambient {1.0f, 0.5f, 0.31f};
    float IMGUIClass::mat_ambient_intensity {1.0f};
    glm::vec3 IMGUIClass::mat_diffuse {1.0f, 0.5f, 0.31};
    float IMGUIClass::mat_diffuse_intensity {1.0f};
    glm::vec3 IMGUIClass::mat_specular {0.5f};
    float IMGUIClass::mat_specular_intensity {1.0f};


    std::vector<float> IMGUIClass::shininess_list  {2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f, 256.0f};
    int IMGUIClass::light_Select_Id = 0;

    std::string IMGUIClass::destroy_name_preview {""};
    int IMGUIClass::destroy_select_id = 0;
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
                if (entity_buf.empty()) entity_buf = "Entity";
                EntityMessage msg(MessageType::CreateEntity, entity_buf);
                msg.texture = TextureManager::Get()->GetTextures()[0];
                msg.spec_texture = TextureManager::Get()->GetTextures()[1];
                msg.file = OBJLoader::FilesSerialized[4];
                EntityManager::Get().ProcessMessages(msg);
            }

            if (!EntityManager::Get().GetEntityList().empty()) {
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
            }


            for (auto & e : EntityManager::Get().GetEntityList()) {
                if (ImGui::CollapsingHeader(e->name.c_str(), ImGuiTreeNodeFlags_None)) {
                    glm::vec3 translation = e->GetPosition();
                    std::string posName = "Position##";
                    if (ImGui::DragFloat3(posName.c_str(), value_ptr(translation),
                        0.0f, -10.0f, 10.0f, "%.2f")) {
                        e->SetPostion(translation);
                        }
                    glm::vec3 scale = e->GetScale();
                    std::string sclName = "Scale##";
                    if (ImGui::DragFloat3(sclName.c_str(), glm::value_ptr(scale),
                        0.0f, 0.0f, 10.0f, "%.2f")) {
                        e->SetScale(scale);
                        }
                    glm::vec3 rotation = e->GetRotation();
                    std::string sroName = "Rotation##";
                    if (ImGui::DragFloat3(sroName.c_str(), glm::value_ptr(rotation),
                        0, -360, 360)) {
                        e->SetRotation(rotation);
                        }

                    if (ImGui::BeginCombo("Shininess", std::to_string(e->shininess).c_str())) {
                        for (int i = 0; i < shininess_list.size(); i++) {
                            bool selected = light_Select_Id == i;
                            if (ImGui::Selectable(std::to_string(shininess_list[i]).c_str(), selected)) {
                                light_Select_Id = i;
                                e->shininess = shininess_list[i];
                                shader.SetFloat("shininess", e->shininess);
                            }
                        }
                        ImGui::EndCombo();
                    }

                    if (ImGui::Checkbox("Simulate?##", &e->simulate)) {
                        if (e->simulate) {
                            Physics::Get()->AddEntityToSimulate(e);
                        }
                        if (!e->simulate) {
                            Physics::Get()->RemoveEntityToSimulate(e);
                        }
                    }
                    ImGui::Checkbox("Has Gravity##", &e->collider.hasGravity);
                    ImGui::DragFloat("GravityModifier##",
                        &Physics::Get()->gravityModifier, .1);

                    if (ImGui::BeginCombo("CollisionType##",
                        ToCString(e->collision_type))) {
                        CollisionTypes type = CollisionTypes::NoCollision;
                        bool selected = type == e->collision_type;
                        if (ImGui::Selectable(ToCString(type), selected)) {
                            e->collision_type = type;
                        }
                        type = CollisionTypes::SphereCollision;
                        selected = type == e->collision_type;
                        if (ImGui::Selectable(ToCString(type), selected)) {
                            e->collision_type = type;
                        }
                        type = CollisionTypes::BoxCollision;
                        selected = type == e->collision_type;
                        if (ImGui::Selectable(ToCString(type), selected)) {
                            e->collision_type = type;
                        }
                        type = CollisionTypes::RayCollision;
                        selected = type == e->collision_type;
                        if (ImGui::Selectable(ToCString(type), selected)) {
                            e->collision_type = type;
                        }
                        type = CollisionTypes::RayABBBCollision;
                        selected = type == e->collision_type;
                        if (ImGui::Selectable(ToCString(type), selected)) {
                            e->collision_type = type;
                        }
                        ImGui::EndCombo();
                    }

                    switch (e->collision_type) {
                        case CollisionTypes::SphereCollision:
                            ImGui::DragFloat("Sphere radius##",
                                &e->collider.radius, .1f);
                            break;
                        case CollisionTypes::BoxCollision:
                            ImGui::DragFloat3("Box Extents##",
                                glm::value_ptr(e->collider.extent), .1);
                            break;
                        case CollisionTypes::RayCollision:
                            ImGui::DragFloat3("RayDirection##",
                                glm::value_ptr(e->collider.direction), .1f, -1, 1);
                        default: break;
                    }
                }
                shader.SetFloat("shininess", e->shininess);

            }
            ImGui::End();
    }

    void IMGUIClass::CameraWindow(Camera& camera) {
        if (showDemoWindow) return;
        if (ImGui::CollapsingHeader("Camera")) {
            glm::vec3 camPos = camera.GetCameraPos();
            if (ImGui::DragFloat3("Camera Position", glm::value_ptr(camPos),
                0.0f, -100.0f, 100.0f, "%.2f")) {
                camera.SetCameraPos(camPos);
                }

            glm::vec3 camRot = camera.GetCameraRotation();
            if (ImGui::DragFloat3("Camera Rotation", glm::value_ptr(camRot),
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

    void ComboTextureFiltering(const char* name, TextureFilter* filter) {

        if (ImGui::BeginCombo(name, ToCString(*filter))) {
            auto e_nearest = TextureFilter::Nearest;
            bool nearest = *filter == e_nearest;
            if (ImGui::Selectable(ToCString(e_nearest), nearest)) {
                *filter = e_nearest;
            }
            auto e_linear = TextureFilter::Linear;
            bool linear = *filter == e_linear;
            if (ImGui::Selectable(ToCString(e_linear), linear)) {
                *filter = e_linear;
            }
            auto e_NMN = TextureFilter::NearestMipmapNearest;
            bool NMN = *filter == e_NMN;
            if (ImGui::Selectable(ToCString(e_NMN), NMN)) {
                *filter = e_NMN;
            }
            auto e_LML = TextureFilter::LinearMipmapLinear;
            bool LML = *filter == e_LML;
            if (ImGui::Selectable(ToCString(e_LML), LML)) {
                *filter = e_LML;
            }
            auto e_NML = TextureFilter::NearestMipmapLinear;
            bool NML = *filter == e_NML;
            if (ImGui::Selectable(ToCString(e_NML), NML)) {
                *filter = e_NML;
            }
            auto e_LMN = TextureFilter::LinearMipmapNearest;
            bool LMN = *filter == e_LMN;
            if (ImGui::Selectable(ToCString(e_LMN), LMN)) {
                *filter = e_LMN;
            }
            ImGui::EndCombo();
        }
    }

    void IMGUIClass::TexturesWindow() {
        if (showDemoWindow) return;

        if (ImGui::Begin("Texture Window")) {

            ImGui::Dummy(ImVec2(0, 5));

            auto entity = EntityManager::Get().GetEntityList()[textureCombo_select_id];
            if (ImGui::BeginCombo("Entity Select", entity->name.c_str())) {
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

            ImGui::Dummy(ImVec2(0, 5));
            ImGui::Text("Texture Settings");
            ImGui::Dummy(ImVec2(0, 5));

            size_t size = TextureManager::Get()->GetTextures().size();
            if (ImGui::BeginListBox("Textures List", {150, size * 20.0f})) {
                for(int i = 0; i < size; i++) {
                    const bool isSelected = i == textures_select_id;
                    if (ImGui::Selectable(
                        TextureManager::Get()->GetTextures()[i].fileName.c_str(), isSelected)) {
                        textures_select_id = i;
                    }
                }
                ImGui::EndListBox();
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
                ImGui::EndListBox();
            }


            if (ImGui::Button("Change Specular Texture")) {
                auto ent = EntityManager::Get().GetEntityList()[textureCombo_select_id];
                ent->spec_texture = TextureManager::Get()->GetTextures()[spec_texture_select_id];
            }

            ImGui::Dummy({0, 10});

            if (ImGui::Button("Change MipMap Settings")) {
                TextureManager::Get()->ChangeMipMapSettings(entity->texture);
            }
            auto wrap = entity->texture.GetWrapping();
            if (ImGui::BeginCombo("Wrapping",
                ToCString(wrap))) {

                bool repeat = wrap == TextureWrapping::Repeat;
                if (ImGui::Selectable(ToCString(TextureWrapping::Repeat), repeat)) {
                    entity->texture.SetWrapping(TextureWrapping::Repeat);
                }
                bool mirrorrepeat = wrap == TextureWrapping::MirroredRepeat;
                if (ImGui::Selectable(ToCString(TextureWrapping::MirroredRepeat), mirrorrepeat)) {
                    entity->texture.SetWrapping(TextureWrapping::MirroredRepeat);
                }
                bool clamptoedge = wrap == TextureWrapping::ClampToEdge;
                if (ImGui::Selectable(ToCString(TextureWrapping::ClampToEdge), clamptoedge)) {
                    entity->texture.SetWrapping(TextureWrapping::ClampToEdge);
                }
                bool clamptoborder = wrap == TextureWrapping::ClampToBorder;
                if (ImGui::Selectable(ToCString(TextureWrapping::ClampToBorder), clamptoborder)) {
                    entity->texture.SetWrapping(TextureWrapping::ClampToBorder);
                }
                ImGui::EndCombo();
            }

            TextureFilter min = entity->texture.GetFilteringMin();
            ComboTextureFiltering("Min", &min);
            entity->texture.SetFilteringMin(min);

            TextureFilter max = entity->texture.GetFilteringMax();
            ComboTextureFiltering("Max", &max);
            entity->texture.SetFilteringMax(max);
        }
        ImGui::End();
    }

    void IMGUIClass::LightsWindow(Shader& shader) {
        if (showDemoWindow) return;
        if (ImGui::Begin("Lights Window")) {
            for (auto& lightEntity : LightEntityManager::Get().GetAllLights()) {
                std::string name = lightEntity->name;
                if (ImGui::CollapsingHeader(name.c_str())) {

                    auto pos = lightEntity->GetPosition();
                    if (ImGui::DragFloat3("Position##", value_ptr(pos),
                        1)) {
                        lightEntity->SetPosition(pos);
                        }

                    auto scale = lightEntity->GetScale();
                    if (ImGui::DragFloat3("Scale##", value_ptr(scale),
                        1)) {
                        lightEntity->SetScale(scale);
                        }

                    combo_type = lightEntity->GetLightType();
                    if (ImGui::BeginCombo("LightType##", ToCString(combo_type))) {
                        bool point_select = combo_type == LightTypes::PointLight;
                        if (ImGui::Selectable(ToCString(LightTypes::PointLight), point_select)) {
                            combo_type = LightTypes::PointLight;
                            lightEntity->ChangeLightType(combo_type);
                        }
                        bool spot_select = combo_type == LightTypes::SpotLight;
                        if (ImGui::Selectable(ToCString(LightTypes::SpotLight), spot_select)) {
                            combo_type = LightTypes::SpotLight;
                            lightEntity->ChangeLightType(combo_type);
                        }
                        bool dir_select = combo_type == LightTypes::DirectionalLight;
                        if (ImGui::Selectable(ToCString(LightTypes::DirectionalLight), dir_select)) {
                            combo_type = LightTypes::DirectionalLight;
                            lightEntity->ChangeLightType(combo_type);
                        }
                        ImGui::EndCombo();
                    }
                    glm::vec3 dir;
                    float cutOff, outer_cutOff;
                    switch (combo_type) {
                        case LightTypes::PointLight:

                            break;
                        case LightTypes::SpotLight:
                            dir = lightEntity->GetSpotLightDirection();
                            if (ImGui::DragFloat3("Light Direction##",
                            value_ptr(dir), 0.1f, -360, 360)) {
                                lightEntity->SetSpotLightDirection(shader, dir);
                            }

                            cutOff = lightEntity->GetCutOff();
                            if (ImGui::DragFloat("Light Cutoff##", &cutOff,
                                0.5, 0, 180)) {
                                lightEntity->SetCutOff(shader, cutOff);
                            };

                            outer_cutOff = lightEntity->GetOuterCutOff();
                            if (ImGui::DragFloat("Light Outer Cutoff##", &outer_cutOff,
                                 0.5, 0, 180)) {
                                lightEntity->SetOuterCutOff(shader, outer_cutOff);
                            }
                            break;

                        case LightTypes::DirectionalLight:
                            dir = lightEntity->GetDirectionalLightDirection();
                            if (ImGui::DragFloat3("Light Direction##",
                                value_ptr(dir), 0.1f, -360 , 360)) {
                                lightEntity->SetDirectionalLightDirection(shader, dir);
                            }
                            break;
                    }

                    ImGui::Dummy({0, 10});
                    ImGui::Text("Light values:");
                    glm::vec3 ambient = lightEntity->GetAmbient();
                    float i_ambient = lightEntity->GetAmbinetIntensity();
                    ImGui::ColorEdit3("Ambient##", value_ptr(ambient));
                    ImGui::DragFloat("AmbientIntensity##",
                        &i_ambient, 0.1, 1, 5);
                    lightEntity->SetAmbientIntensity(i_ambient);
                    lightEntity->SetAmbient(ambient);

                    glm::vec3 diffuse = lightEntity->GetDiffuse();
                    float i_diffuse = lightEntity->GetDiffuseIntensity();
                    ImGui::ColorEdit3("Diffuse##", value_ptr(diffuse));
                    ImGui::DragFloat("DiffuseIntensity##",
                        &i_diffuse, .1, 1, 5);
                    lightEntity->SetDiffuseIntensity(i_diffuse);
                    lightEntity->SetDiffuse(diffuse);

                    glm::vec3 specular = lightEntity->GetSpecular();
                    float i_specular = lightEntity->GetSpecularIntensity();
                    ImGui::ColorEdit3("Specular##", value_ptr(specular));
                    ImGui::DragFloat("SpecularIntensity##",
                        &i_specular, .1, 1, 5);
                    lightEntity->SetSpecularIntensity(i_specular);
                    lightEntity->SetSpecular(specular);
                }
                lightEntity->CombineModels();
            }
            ImGui::Dummy({0, 10});
            ImGui::Text("Light Entity Create and Destroy");
            if (ImGui::BeginCombo("LightType##", ToCString(create_combo_type))) {
                bool point_select = create_combo_type == LightTypes::PointLight;
                if (ImGui::Selectable(ToCString(LightTypes::PointLight), point_select)) {
                    create_combo_type = LightTypes::PointLight;
                }
                bool spot_select = create_combo_type == LightTypes::SpotLight;
                if (ImGui::Selectable(ToCString(LightTypes::SpotLight), spot_select)) {
                    create_combo_type = LightTypes::SpotLight;
                }
                bool dir_select = create_combo_type == LightTypes::DirectionalLight;
                if (ImGui::Selectable(ToCString(LightTypes::DirectionalLight), dir_select)) {
                    create_combo_type = LightTypes::DirectionalLight;
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Create Light##")) {
                LightEntityManager::Get().CreateLight(create_combo_type);
            }
            if (!LightEntityManager::Get().GetAllLights().empty()) {
                destroy_name_preview = LightEntityManager::Get().GetAllLights()[destroy_select_id]->name;
                if (ImGui::BeginCombo("Light Entity##", destroy_name_preview.c_str())) {
                    for (int i = 0; i < LightEntityManager::Get().GetAllLights().size(); i++) {
                        auto ent = LightEntityManager::Get().GetAllLights()[i];
                        bool selected = i == destroy_select_id;
                        if (ImGui::Selectable(ent->name.c_str(), selected)) {
                            destroy_select_id = i;
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                if (ImGui::Button("Destroy Light")) {
                    LightEntityManager::Get().DestroyLightByName(destroy_name_preview);
                    if (destroy_select_id != 0) {
                        destroy_select_id -= 1;
                    }
                }
            }

        }
        ImGui::End();
    }

    glm::vec3 IMGUIClass::force = glm::vec3(0);

    void IMGUIClass::PhysicsWindow() {
        if (ImGui::Begin("Physics Window")) {
            if (ImGui::DragFloat3("Forces", value_ptr(force), 0.1, -1, 1)) {

            }
            ImGui::Checkbox("Simulate", &run_simulation);
            if (run_simulation) {

            }

        }
        ImGui::End();
    }

}
