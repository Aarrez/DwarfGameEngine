#include "GameEngine.h"

#include "../Managers/LightEntityManager.h"

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
        LightEntityManager::Allocate();
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
        Mesh mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(mainShader, mesh);

        EntityMessage entMsg(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[1];
        entMsg.texture = TextureManager::Instance()->GetTextures()[0];
        EntityManager::ProcessMessages(entMsg);

        LightEntityManager::Get().CreateLight(LightTypes::PointLight);
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

        //Non-light Objects
        virtual_object->SetLightUniforms(*mainShader);
        glm::mat4 projection {glm::mat4(1.0f)};
        projection = glm::perspective(radians(45.0f),
            static_cast<float>(Width) / Height, 0.1f, 100.0f);
        mat4 view {mat4(1.0f)};
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        mainShader->SetMatrix4("view", view);
        mainShader->SetMatrix4("projection", projection);

        camera->MoveCamera(Input::GetMoveValue(), 0.1);

        mainShader->UseShaderProgram();

        for (auto & i : EntityManager::GetEntityList()) {
            mainShader->SetMatrix4("model", i->transform);
            Mesh m = MeshManager::Instance()->FindMesh(i->meshName);
            virtual_object->SetVertexBufferObjects(m);
            TextureManager::Instance()->DrawTexture(i->texture);
            virtual_object->SetTextureUnit(i->texture.textureID);
            virtual_object->Draw(virtual_object->VAO);
        }

        for (auto & i : LightEntityManager::Get().GetAllLights()) {
            //SetVertexBufferObjects here for light objects later

            virtual_object->SetLightUniforms(*lightShader);
            i->SetModelMatrix(*lightShader);
            i->SetView(glm::mat4(view));
            i->SetProjection(glm::mat4(projection));
            virtual_object->Draw(virtual_object->lightVAO);
        }


        IMGUIClass::InitUpdateLoop();

        IMGUIClass::MenuBar();

        IMGUIClass::CameraWindow(*camera);

        IMGUIClass::EntityWindow();

        IMGUIClass::ModelsWindow();

        IMGUIClass::TexturesWindow();

        IMGUIClass::LightsWindow();

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
