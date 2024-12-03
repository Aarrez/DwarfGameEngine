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

        File::DwarfPathChange::ChangeCurrentPathToProjectRoot();
        window = glfwCreateWindow(Width, Height, "DwarfEngine", nullptr, nullptr);
        if (window == nullptr) {
            /*std::cerr << "Failed to create GLFW window" << std::endl;*/
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        /*dwarf_input = std::make_shared<DwarfInput>(window);*/
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return;
        }

        glViewport(0, 0, Width, Height);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }


    void DwarfEngine::Init() {
        DwarfInput::Allocate(window);


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
        /*std::cout << "Current available phys memory" << to_string(Memory::GetPhysicalMemoryAvailable()) << std::endl;*/
        std::string filename = "DwarfModels/OBJFiles/teapot.obj";
        vector<Vertex> vertex_vector = DwarfOBJLoader::GetVerticesFromOBJ(filename);

        dwarfMesh2D = make_unique<Mesh2D::DwarfMesh2D>(shader, vertex_vector);

        DEM = make_unique<DwarfEntityManager>();

        dwarfMesh2D->SetTextureUnit();

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

        const float cameraSpeed = deltaTime * 2.5f;

        /*camera->MoveCamera(DwarfInput::GetMoveValue()->ToVec3(), cameraSpeed);*/

        glm::mat4 model {glm::mat4(1.0f)};
        glm::mat4 projection {glm::mat4(1.0f)};
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Width) / Height, 0.1f, 100.0f);
        mat4 view {mat4(1.0f)};
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        model = glm::scale(model, model_scale);
        model = glm::translate(model, model_position);
        if (model_rotation != vec3(0.0f)) {
            model = glm::rotate(model, glm::radians(rad), model_rotation);
        }



        /*camera->RotateCameraWithTime(glfwGetTime(), .1f);*/

        shader->SetMatrix4("projection", 1, GL_FALSE, projection);
        shader->SetMatrix4("view", 1, GL_FALSE, view);



        shader->UseShaderProgram();

        for (unsigned int i = 0; i < AmountOfMeshes; i++) {
            mat4 tempmodel = mat4(1.0f);
            tempmodel = translate(tempmodel, cubePositions[i] + model_position);
            tempmodel = scale(tempmodel, model_scale);
            float angle = 20.0f * i;
            tempmodel = rotate(tempmodel, glm::radians(angle), vec3(1.f, .3f, .5f));
            shader->SetMatrix4("model", 1, GL_FALSE, tempmodel);
            dwarfMesh2D->Draw(shader);

        }

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("DwarfDemoWindow", &show_demo_window);
            if (ImGui::Button("Button")) {
                DwarfInput::GetMoveValue();
            }

            ImGui::Text("FPS: %.2f\n", ImGui::GetIO().Framerate);
            ImGui::Text("Change window showed\n");
            ImGui::Checkbox("Show demo window", &show_demo_window);
            ImGui::Checkbox("Show another window", &show_another_window);
            ImGui::DragFloat3
            ("Position", value_ptr(model_position),1.0f,-10.0f, 10.0f);
            ImGui::DragFloat3
            ("Rotation axis", value_ptr(model_rotation), 1.0f, 0.0f, 1.0f);
            ImGui::DragFloat
            ("Radians", &rad, 1.0f, 0.0f, 360.0f);
            ImGui::DragFloat3
            ("Scale", value_ptr(model_scale), .1f, 0.1f, 2.0f);

            ImGui::Text("Change the background color");
            ImGui::ColorEdit3("clear color", (float*)&clear_color);

            ImGui::Text("Application avrage %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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




