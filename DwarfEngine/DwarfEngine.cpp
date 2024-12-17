#include "DwarfEngine.h"



namespace Dwarf {

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    DwarfEngine::DwarfEngine(): window(nullptr) {
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
            /*std::cerr << "Failed to create GLFW window" << std::endl;*/
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


    void DwarfEngine::Init() {
        DwarfOBJLoader::GetBinaryFiles();
        DwarfInput::Allocate(window);
        DwarfEntityManager::Allocate();


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
        shader = std::make_shared<DwarfShader>();

        camera = make_unique<Camera::DwarfCamera>(shader);
        std::string filePath = "DwarfModels/OBJFiles/Cube.obj";
        auto meshData = DwarfOBJLoader::OBJFileParser(filePath);
        DwarfOBJLoader::OBJDataSerializer(filePath, meshData.value());
        std::string s = "Cube";
        MeshData data = DwarfOBJLoader::OBJDataDeserializer(s);

        vector<Vertex> vertex_vector = DwarfOBJLoader::GetVerticesFromData(data);

        dwarfMesh2D = make_unique<DwarfMesh2D>(shader, vertex_vector);

        dwarfMesh2D->SetTextureUnit();

        for (int i = 0; i < AmountOfMeshes; i++) {
            DwarfEntityManager::CreateEntity();
        }
    }

    void DwarfEngine::Update() {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwPollEvents();
    }

    void DwarfEngine::Render() {

        glfwGetFramebufferSize(window, &Width, &Height);
        glViewport(0, 0, Width, Height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        dwarfMesh2D->BindOnTextureUnit();


        glm::mat4 model {glm::mat4(1.0f)};
        glm::mat4 projection {glm::mat4(1.0f)};
        projection = glm::perspective(radians(45.0f), static_cast<float>(Width) / Height, 0.1f, 100.0f);
        mat4 view {mat4(1.0f)};
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        shader->SetMatrix4("veiw", 1, GL_FALSE, view);

        model = glm::scale(model, model_scale);
        model = glm::translate(model, model_position);
        if (model_rotation != vec3(0.0f)) {
            model = glm::rotate(model, glm::radians(rad), model_rotation);
        }


        shader->SetMatrix4("projection", 1, GL_FALSE, projection);
        camera->MoveCamera(DwarfInput::GetMoveValue(),
            DwarfInput::GetCameraDirection(),
            .1);


        shader->UseShaderProgram();

        for (auto & i : *DwarfEntityManager::GetEntityList()) {

            shader->SetMatrix4("model", 1, GL_FALSE,
                i->transform);
            dwarfMesh2D->Draw(shader);
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

            if (ImGui::Button("Create Entity")) {
                DwarfEntityManager::CreateEntity();
            }

            ImGui::InputText("Entity Name", &ent_buf);
            if (ImGui::Button("Destroy Entity")) {
                DwarfEntityManager::RemoveEntityByName(ent_buf);
            }

            for (auto & e : *DwarfEntityManager::GetEntityList()) {
                if (ImGui::CollapsingHeader(e->name.c_str(), ImGuiTreeNodeFlags_None)) {
                    vec3 translation = e->GetPosition();
                    string posName = "Position" + to_string(e->id);
                    if (ImGui::DragFloat3(posName.c_str(), value_ptr(translation),
                        0.0f, -10.0f, 10.0f, "%.2f")) {
                        e->Translate(translation);
                        }
                    vec3 scale = e->GetScale();
                    string sclName = "Scale" + to_string(e->id);
                    if (ImGui::DragFloat3(sclName.c_str(), value_ptr(scale),
                        0.0f, 0.0f, 10.0f, "%.2f")) {
                        e->SetScale(scale);
                        }
                    vec3 rotation = e->GetRotation();
                    string sroName = "Rotation" + to_string(e->id);
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
                auto data = DwarfOBJLoader::OBJFileParser(file_buf);
                if (!data.has_value())
                    std::cerr << "Not a valid File path: " << file_buf << std::endl;
                else {
                    DwarfOBJLoader::OBJDataSerializer(file_buf, data.value());
                }
            }
            if (ImGui::BeginListBox("Meshes")) {
                for (int i = 0; i < DwarfOBJLoader::FilesSerialized.size(); i++) {
                    const bool is_selected = selected_int == i;
                    if (ImGui::Selectable(DwarfOBJLoader::FilesSerialized[i].fileName.c_str(), is_selected)) {
                        selected_int = i;
                        selected_path = DwarfOBJLoader::FilesSerialized[i].binPath;
                    }
                }
                ImGui::EndListBox();
            }

            preview_ent = DwarfEntityManager::GetEntityList()->at(comb_selected)->name;
            if (ImGui::BeginCombo("Entity Select", preview_ent.c_str())) {
                for (int i = 0; i < DwarfEntityManager::GetEntityList()->size(); i++) {
                    const bool is_selected = comb_selected == i;
                    if (ImGui::Selectable(DwarfEntityManager::GetEntityList()->at(i)->name.c_str(), is_selected)) {
                        comb_selected = i;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Change Entity Mesh")) {
                auto ent = DwarfEntityManager::GetEntityList()->at(comb_selected);
                auto mesh = DwarfOBJLoader::FilesSerialized.at(selected_int);
                ent->model = mesh;
            }

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    void DwarfEngine::Shutdown() {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();

    }

    DwarfEngine::~DwarfEngine() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DwarfEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}




