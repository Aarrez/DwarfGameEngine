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
        ShadowManager::Allocate();
    }

    void GameEngine::InitializeGL() {
        glEnable(GL_DEPTH_TEST);
        IMGUIClass::InitImGui(window);
    }

    void GameEngine::Init() {

        mainShader = std::make_shared<Shader>(
            "ShaderScripts/Misc/VertexShader.vert",
            "ShaderScripts/Misc/FragmentShader.frag");
        lightShader = std::make_shared<Shader>(
            "ShaderScripts/Light/LightingVertexShader.glsl",
            "ShaderScripts/Light/LightingFragmentShader.glsl");
        simpleDepthShader = std::make_shared<Shader>(
            "ShaderScripts/Shadow/SimpleDepthShader.vert",
            "ShaderScripts/Shadow/SimpleDepthShader.frag");

        TextureManager::Get()->StbGenerateTextures();
        camera = std::make_unique<Camera>();
        Input::SetCameraRef(camera.get());
        std::string s = "TheCube.bin";
        cube_mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(mainShader, cube_mesh);
        virtual_object->BindLightVAO();


        EntityMessage entMsg(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[5];
        entMsg.texture = TextureManager::Get()->GetTextures()[0];
        entMsg.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg);

        EntityMessage entMsg2(MessageType::CreateEntity, "Entity");
        entMsg2.file = OBJLoader::FilesSerialized[1];
        EntityManager::Get().ProcessMessages(entMsg2);
        auto* entlist = &EntityManager::Get().GetEntityList();
        for(int i = 0; i < entlist->size(); i++) {
            entlist->at(i)->Translate({0, -3 * i, 0});
        }

        LightTypes type {LightTypes::PointLight};
        auto ent = LightEntityManager::Get().CreateLight(type);
        ent->SetPosition({3.0f, 2.0f, 2.0f});
        ShadowManager::Get()->GenerateDepthMap(&shadow_map);

        TextureManager::Get()->SetTextureUniform(*mainShader);
    }

    void GameEngine::Update() {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwPollEvents();
    }

    void GameEngine::Render() {

        glfwGetFramebufferSize(window, &Width, &Height);


        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float near_plane = 1.0f;
        float far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f,
          -10.0f, 10.0f,
            near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(
          glm::vec3(-2.0f, 4.0f, -1.0f),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightView * lightProjection;
        simpleDepthShader->UseShaderProgram();
        simpleDepthShader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, shadow_map.GetShadowWidth(), shadow_map.GetShadowHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_map.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        TextureManager::Get()->DrawTexture(TextureManager::Get()->GetTextures()[0]);
        TextureManager::Get()->DrawTexture(TextureManager::Get()->GetTextures()[1]);
        /*RenderScene(*simpleDepthShader);*/
        for (auto & i : LightEntityManager::Get().GetAllLights()) {
            i->SetModelMatrix(*simpleDepthShader);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glViewport(0, 0, Width, Height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mainShader->UseShaderProgram();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f),
            static_cast<float>(Width) / Height, 0.1f, 100.0f);
        glm::mat4 view {glm::mat4(1.0f)};
        camera->MoveCamera(Input::GetMoveValue(), 0.1, &view);
        view = translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        TextureManager::Get()->SetTextureUniform(*mainShader);
        LightEntityManager::Get().SetLightsUniforms(*mainShader);
        mainShader->SetVector3("viewPos", camera->GetCameraPos());


        mainShader->SetInt("nr_pointlights",
                    LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::PointLight));
        mainShader->SetInt("nr_dirlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::DirectionalLight));
        mainShader->SetInt("nr_spotlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::SpotLight));

        mainShader->SetMatrix4("view", view);
        mainShader->SetMatrix4("projection", projection);
        mainShader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

        TextureManager::Get()->DrawTexture(TextureManager::Get()->GetTextures()[0]);
        TextureManager::Get()->DrawTexture(TextureManager::Get()->GetTextures()[1]);
        ShadowManager::Get()->DrawShadowMap(shadow_map);
        RenderScene(*mainShader);

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

    void GameEngine::RenderScene(Shader& shader) {
        for (auto & i : EntityManager::Get().GetEntityList()) {
            i->SetTransformMatrix();
            shader.SetMatrix4("model", i->transform);
            Mesh m = MeshManager::Instance()->FindMesh(i->meshName);
            virtual_object->SetVertexBufferObjects(m);

            virtual_object->Draw(virtual_object->VAO);
        }

    }



}
