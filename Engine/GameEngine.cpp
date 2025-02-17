#include "GameEngine.h"

namespace Engine {
    GameEngine::GameEngine(): window(nullptr) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        DwarfPathChange::ChangeCurrentPathToProjectRoot();
        window = glfwCreateWindow(Width, Height, "DwarfEngine", nullptr, nullptr);
        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return;
        }

        glViewport(0, 0, Width, Height);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    void GameEngine::Allocate() {
        MeshManager::Allocate();
        OBJLoader::GetBinaryFiles();
        Input::Allocate(window);
        TextureManager::Allocate();
        ThreadManager::Allocate();
        EntityManager::Allocate();
    }

    void GameEngine::InitializeGL() {
        glEnable(GL_DEPTH_TEST);
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

    void GameEngine::Init() {

        shader = std::make_shared<Shader>(
            "ShaderScripts/VertexShader.glsl",
            "ShaderScripts/FragmentShader.glsl");

        lightShader = std::make_shared<Shader>(
            "ShaderScripts/LightingVertexShader.glsl",
            "ShaderScripts/LightingFragmentShader.glsl");

        camera = make_unique<Camera>(shader);
        Input::SetCameraRef(camera.get());
        string s = "bear.bin";
        MeshMessage mmsg(MessageType::AddMesh, s);
        MeshManager::ProcessMessage(&mmsg);
        Mesh mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(shader, mesh);

        virtual_object->SetTextureUnit();
        Texture tex {};
        tex.colorFormat = GL_RGB;
        tex.filePath = "Images/container.jpg";
        for (int i = 0; i < AmountOfMeshes; i++) {
            EntityMessage entMsg(MessageType::CreateEntity, "Entity");
            entMsg.file = OBJLoader::FilesSerialized[0];
            entMsg.texture = tex;
            EntityManager::ProcessMessages(entMsg);
        }
    }

    void GameEngine::Update() {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwPollEvents();
    }

    void GameEngine::Render() {

        glfwGetFramebufferSize(window, &Width, &Height);
        glViewport(0, 0, Width, Height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        virtual_object->BindOnTextureUnit();

        glm::mat4 model {glm::mat4(1.0f)};
        glm::mat4 projection {glm::mat4(1.0f)};
        projection = glm::perspective(radians(45.0f),
            static_cast<float>(Width) / Height, 0.1f, 100.0f);
        mat4 view {mat4(1.0f)};
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        shader->SetMatrix4("veiw", 1, GL_FALSE, view);

        model = glm::scale(model, model_scale);
        model = glm::translate(model, model_position);
        if (model_rotation != vec3(0.0f)) {
            model = glm::rotate(model, glm::radians(rad), model_rotation);
        }

        shader->SetMatrix4("projection", 1, GL_FALSE, projection);
        camera->MoveCamera(Input::GetMoveValue(),
            .1);


        shader->UseShaderProgram();

        for (auto & i : EntityManager::GetEntityList()) {

            shader->SetMatrix4("model", 1, GL_FALSE,
                i->transform);
            Mesh m = MeshManager::Instance()->FindMesh(i->meshName);
            virtual_object->SetVertexBufferObjects(m);
            virtual_object->Draw();
        }

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            ImGui::Checkbox("DemoWindow", &show_demo_window);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
        if (show_demo_window)
            ImGui::ShowDemoWindow();

        if (bool demo_window_Closed = !show_demo_window) {
            ImGui::Begin("EntityWindow");

            if (ImGui::CollapsingHeader("Camera")) {
                vec3 camPos = camera->GetCameraPos();
                if (ImGui::DragFloat3("Camera Position", value_ptr(camPos),
                    0.0f, -100.0f, 100.0f, "%.2f")) {
                    camera->SetCameraPos(camPos);
                }

                vec3 camRot = camera->GetCameraRotation();
                if (ImGui::DragFloat3("Camera Rotation", value_ptr(camRot),
                    0, -1, 1)) {
                    camera->SetCameraRotation(camRot);
                    }
            }

            ImGui::InputText("Entity Name", &ent_buf);
            if (ImGui::Button("Create Entity")) {
                Texture tex {};
                tex.filePath = "Images/container.jpg";
                tex.colorFormat = GL_RGB;
                if (ent_buf == "") ent_buf = "Entity";
                EntityMessage msg(MessageType::CreateEntity, ent_buf);
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

        if (ImGui::Begin("Serialized File")) {

            ImGui::InputText("Path to OBJ File", &file_buf);
            if (ImGui::Button("Serialize File")) {
                MeshMessage meshmsg(MessageType::AddMesh, file_buf);
                MeshManager::ProcessMessage(&meshmsg);
            }
            if (ImGui::BeginListBox("Meshes")) {
                for (int i = 0; i < OBJLoader::FilesSerialized.size(); i++) {
                    const bool is_selected = selected_int == i;
                    if (ImGui::Selectable(OBJLoader::FilesSerialized[i].fileName.c_str(), is_selected)) {
                        selected_int = i;
                        selected_path = OBJLoader::FilesSerialized[i].binPath;
                    }
                }
                ImGui::EndListBox();
            }

            preview_ent = EntityManager::GetEntityList()[comb_selected]->name;
            if (ImGui::BeginCombo("Entity Select", preview_ent.c_str())) {
                for (int i = 0; i < EntityManager::GetEntityList().size(); i++) {
                    const bool is_selected = comb_selected == i;
                    if (ImGui::Selectable(EntityManager::GetEntityList()[i]->name.c_str(), is_selected)) {
                        comb_selected = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Change Entity Mesh")) {
                auto ent = EntityManager::GetEntityList()[comb_selected];
                const auto& meshFileInfo = OBJLoader::FilesSerialized.at(selected_int);
                ent->meshName = MeshManager::Instance()->FindMesh(meshFileInfo.fileName).name;
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    void GameEngine::Shutdown() {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    GameEngine::~GameEngine() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void GameEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}