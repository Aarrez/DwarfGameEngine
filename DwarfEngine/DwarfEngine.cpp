#include "DwarfEngine.h"



namespace Dwarf {
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


        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return;
        }

        glViewport(0, 0, Width, Height);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }


    void DwarfEngine::Init() {

        glEnable(GL_DEPTH_TEST);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();

        if (ImGui_ImplGlfw_InitForOpenGL(window, true) == false) {
            std::cerr << "Failed to init glfw for imgui" << std::endl;
        }
        if (ImGui_ImplOpenGL3_Init("#version 330") == false) {
            std::cerr << "Failed to initialize OpenGL for imgui" << std::endl;
        }

        shader = new DwarfShader();

        //TODO figure out how to draw and what path to give
        /*dwarfModel = new Mesh::DwarfModel("");*/

        camera = new Camera::DwarfCamera();
        std::string some = "DwarfModels/OBJFiles/teapot.obj";
        auto out = DwarfOBJLoader::DwarfVerticesParser(some);
        dwarfMesh2D = new Mesh2D::DwarfMesh2D(shader, std::get<0>(out), std::get<1>(out));

        DEM = new DwarfEntityManager();

        /*for(int i {0}; i < 10; i++){
            auto entity = DEM->CreateDwarfEntity();
            entity.transform = glm::translate(entity.transform, glm::vec3(0,0, i+1));
            entity.transform =
                    glm::rotate(entity.transform, glm::radians(40.0f + i*2), glm::vec3(0,0,1));
        }*/
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
        glm::mat4 view {glm::mat4(1.0f)};
        glm::mat4 projection {glm::mat4(1.0f)};

        shader->SetMatrix4("model", 1, GL_FALSE, model);
        shader->SetMatrix4("view", 1, GL_FALSE, view);
        shader->SetMatrix4("projection", 1, GL_FALSE, projection);


        shader->UseShaderProgram();
        /*for (unsigned int i = 0; i < DEM->entities.capacity() -1; i++) {
            DEM->entities.at(i)->transform = glm::translate(DEM->entities.at(i)->transform, glm::vec3(i + 1, i + 1, i - 1));
            shader->SetMatrix4("model", 1, GL_FALSE, DEM->entities.at(i)->transform);
        }*/

        /*for(Entity &entity : *DEM->GetEntityList()){
            shader->SetMatrix4("moodel", 1, GL_FALSE, entity.transform);

        }*/

        dwarfMesh2D->Draw(shader);
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("DwarfDemoWindow", &show_demo_window);
            ImGui::Text("FPS: %.2f\n", ImGui::GetIO().Framerate);
            ImGui::Text("Change window showed\n");
            ImGui::Checkbox("Show demo window", &show_demo_window);
            ImGui::Checkbox("Show another window", &show_another_window);

            ImGui::Text("Change the background color");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);

            ImGui::Text("Application avrage %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        /*dwarfModel->Draw(shader);*/


        glfwSwapBuffers(window);

    }

    void DwarfEngine::Shutdown() {
        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();

    }

    DwarfEngine::~DwarfEngine() {
        delete dwarfMesh2D;
        delete shader;
        delete DEM;
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DwarfEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}




