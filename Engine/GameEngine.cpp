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
        IMGUIClass::InitImGui(window);
    }

    void GameEngine::Init() {

        mainShader = std::make_shared<Shader>(
            "ShaderScripts/Misc/VertexShader.glsl",
            "ShaderScripts/Misc/FragmentShader.glsl");
        lightShader = std::make_shared<Shader>(
            "ShaderScripts/Light/LightingVertexShader.glsl",
            "ShaderScripts/Light/LightingFragmentShader.glsl");


        TextureManager::Instance()->GenerateTextures();
        camera = make_unique<Camera>(mainShader);
        Input::SetCameraRef(camera.get());
        string s = "bear.bin";
        MeshMessage mMsg(MessageType::AddMesh, s);
        MeshManager::ProcessMessage(mMsg);
        Mesh mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(mainShader, mesh);


        EntityMessage entMsg(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[0];
        entMsg.texture = TextureManager::Instance()->GetTextures().at(0);
        EntityManager::ProcessMessages(entMsg);
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

        virtual_object->SetLightUniforms(mainShader);
        glm::mat4 model {glm::mat4(1.0f)};
        glm::mat4 projection {glm::mat4(1.0f)};
        projection = glm::perspective(radians(45.0f),
            static_cast<float>(Width) / Height, 0.1f, 100.0f);
        mat4 view {mat4(1.0f)};
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        mainShader->SetMatrix4("veiw", 1, GL_FALSE, view);

        model = glm::scale(model, model_scale);
        model = glm::translate(model, model_position);
        if (model_rotation != vec3(0.0f)) {
            model = glm::rotate(model, glm::radians(rad), model_rotation);
        }

        mainShader->SetMatrix4("projection", 1, GL_FALSE, projection);
        camera->MoveCamera(Input::GetMoveValue(), 0.1);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader->SetMatrix4("Model", 1, GL_FALSE, lightModel);


        mainShader->UseShaderProgram();

        for (auto & i : EntityManager::GetEntityList()) {
            mainShader->SetMatrix4("model", 1, GL_FALSE,
                i->transform);
            Mesh m = MeshManager::Instance()->FindMesh(i->meshName);
            virtual_object->SetVertexBufferObjects(m);
            TextureManager::Instance()->DrawTexture(i->texture);
            virtual_object->SetTextureUnit(i->texture.textureID);
            virtual_object->Draw();
        }

        IMGUIClass::InitUpdateLoop();

        IMGUIClass::MenuBar();

        IMGUIClass::CameraWindow(*camera);

        IMGUIClass::EntityWindow();

        IMGUIClass::ModelsWindow();

        IMGUIClass::TexturesWindow();

        IMGUIClass::EndUpdateLoop();

        glfwSwapBuffers(window);

    }

    void GameEngine::Shutdown() {
        IMGUIClass::EndImGui();
    }

    GameEngine::~GameEngine() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void GameEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}
