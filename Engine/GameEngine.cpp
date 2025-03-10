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

        TextureManager::Get()->StbGenerateTextures();
        camera = std::make_unique<Camera>();
        Input::SetCameraRef(camera.get());
        std::string s = "TheCube.bin";
        Mesh mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(mainShader, mesh);

        EntityMessage entMsg(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[4];
        entMsg.texture = TextureManager::Get()->GetTextures()[0];
        entMsg.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg);

        LightTypes type {LightTypes::PointLight};
        auto ent = LightEntityManager::Get().CreateLight(type);
        ent->SetPosition({3.0f, 2.0f, 2.0f});
        ent->SetScale({0.5f, 0.5f, 0.5f});

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
        glm::mat4 projection {glm::mat4(1.0f)};
        projection = glm::perspective(radians(45.0f),
            static_cast<float>(Width) / Height, 0.1f, 100.0f);
        mat4 view {mat4(1.0f)};
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        TextureManager::Get()->SetTextureUniform(*mainShader);
        mainShader->SetInt("nr_pointlights",
                    LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::PointLight));
        mainShader->SetInt("nr_dirlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::DirectionalLight));
        mainShader->SetInt("nr_spotlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::SpotLight));
        LightEntityManager::Get().SetLightsUniforms(*mainShader);
        mainShader->SetVector3("viewPos", camera->GetCameraPos());
        mainShader->UseShaderProgram();

        camera->MoveCamera(Input::GetMoveValue(), 0.1, &view);
        mainShader->SetMatrix4("view", view);
        mainShader->SetMatrix4("projection", projection);
        for (auto & i : EntityManager::Get().GetEntityList()) {
            mainShader->SetMatrix4("model", i->transform);
            Mesh m = MeshManager::Instance()->FindMesh(i->meshName);
            virtual_object->SetVertexBufferObjects(m);

            TextureManager::Get()->DrawTexture(i->texture);
            TextureManager::Get()->DrawTexture(i->spec_texture);
            virtual_object->Draw(virtual_object->VAO);
        }
        lightShader->UseShaderProgram();
        lightShader->SetMatrix4("view", view);
        lightShader->SetMatrix4("projection", projection);
        for (auto & i : LightEntityManager::Get().GetAllLights()) {
            //SetVertexBufferObjects here for light objects later
            i->SetModelMatrix(*lightShader);
            virtual_object->Draw(virtual_object->lightVAO);
        }

        mainShader->UseShaderProgram();

        IMGUIClass::InitUpdateLoop();

        IMGUIClass::MenuBar();

        IMGUIClass::CameraWindow(*camera);

        IMGUIClass::EntityWindow(*mainShader);

        IMGUIClass::ModelsWindow();

        IMGUIClass::TexturesWindow();

        IMGUIClass::LightsWindow(*mainShader);

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
