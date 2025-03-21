#include "GameEngine.h"

#include "../Managers/LightEntityManager.h"

namespace Engine {
    GameEngine::GameEngine() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif



        window = glfwCreateWindow(Width, Height, "DwarfEngine", nullptr, nullptr);
        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        /*glfwSwapInterval(1);*/

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return;
        }
        glEnable(GL_DEPTH_TEST);


        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        DwarfPathChange::ChangeCurrentPathToProjectRoot();
    }

    void GameEngine::Allocate() {
        LightEntityManager::Allocate();
        MeshManager::Allocate();
        OBJLoader::GetBinaryFiles();
        Input::Allocate(window);
        TextureManager::Allocate();
        ThreadManager::Allocate();
        EntityManager::Allocate();
        Physics::Allocate();
        /*ShadowManager::Allocate();*/
    }

    void GameEngine::InitializeGL() {

        IMGUIClass::InitImGui(window);
    }

    unsigned int depthMap;
    unsigned int depthMapFBO;
    const unsigned int ShadowWidth = 1024, ShadowHeight = 1024;

    void GameEngine::Init() {

        lightShader = std::make_shared<Shader>(
            "ShaderScripts/Misc/VertexShader.vert",
            "ShaderScripts/Misc/FragmentShader.frag");
        simpleDepthShader = std::make_shared<Shader>(
            "ShaderScripts/Shadow/SimpleDepthShader.vert",
            "ShaderScripts/Shadow/SimpleDepthShader.frag");
        debugDepthMapQuad = std::make_shared<Shader>(
        "ShaderScripts/Shadow/debug_quad.vert",
        "ShaderScripts/Shadow/debug_quad_depth.frag");
        /*lightShader = std::make_shared<Shader>(
            "ShaderScripts/Light/LightingVertexShader.vert",
            "ShaderScripts/Light/LightingFragmentShader.frag");*/

        TextureManager::Get()->StbGenerateTextures();

        glGenFramebuffers(1, &depthMapFBO);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        camera = std::make_unique<Camera>();
        Input::SetCameraRef(camera.get());
        std::string s = "TheCube.bin";
        cube_mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(lightShader, cube_mesh);
        virtual_object->BindLightVAO();

        EntityMessage entMsg(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[6];
        entMsg.texture = TextureManager::Get()->GetTextures()[0];
        entMsg.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg);

        EntityMessage entMsg2(MessageType::CreateEntity, "Entity");
        entMsg2.file = OBJLoader::FilesSerialized[2];
        entMsg2.texture = TextureManager::Get()->GetTextures()[0];
        entMsg2.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg2);

        EntityMessage entMsg3(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[6];
        entMsg.texture = TextureManager::Get()->GetTextures()[0];
        entMsg.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg);



        auto* entlist = &EntityManager::Get().GetEntityList();

        for(int i = 0; i < entlist->size(); i++) {
            entlist->at(i)->Translate({0, -2.5 * i, 0});
        }
        /*entlist->at(0)->SetRotation({0, 60, 30});
        entlist->at(0)->SetScale(glm::vec3(.5));*/
        entlist->at(2)->SetPostion(glm::vec3(0, 4, 0));
        entlist->at(0)->simulate = true;
        entlist->at(2)->simulate = true;
        
        Physics::Get()->AddEntityToSimulate(entlist->at(0));
        Physics::Get()->AddEntityToSimulate(entlist->at(2));


        LightTypes type {LightTypes::PointLight};
        auto ent = LightEntityManager::Get().CreateLight(type);
        ent->SetPosition({0.0f, 3.0f, -1.0f});
        ent->CombineModels();


        lightShader->UseShaderProgram();
        lightShader->SetInt("diffuseTexture", 0);
        lightShader->SetInt("shadowMap", 1);
        lightShader->SetInt("specular", 2);


        debugDepthMapQuad->UseShaderProgram();
        debugDepthMapQuad->SetInt("depthMap", 0);

        lastTime = glfwGetTime();
    }

    void GameEngine::Update() {
        nowTime = glfwGetTime();
        deltaTime = (nowTime - lastTime);
        lastTime = nowTime;

        auto phy = Physics::Get();
        phy->Simulate(deltaTime);

        glfwPollEvents();
    }

    void GameEngine::Render() {

        glfwGetFramebufferSize(window, &Width, &Height);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        LightEntityManager::Get().SetLightsUniforms(*simpleDepthShader);
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        //Orthographic
        lightProjection = glm::ortho(-10.0f, 10.0f,
          -10.0f, 10.0f,
            light_near_plane, light_far_plane);
        //Perspective
        /*lightProjection = glm::perspective(glm::radians(45.0f),
            static_cast<GLfloat>(shadow_map.ShadowWidth)/
            static_cast<GLfloat>(shadow_map.ShadowHeight),
            near_plane,
            far_plane);*/

        lightView = glm::lookAt(
          LightEntityManager::Get().GetAllLights()[0]->GetPosition(),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection *lightView;
        simpleDepthShader->UseShaderProgram();
        simpleDepthShader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);


        glCullFace(GL_FRONT);
        glViewport(0, 0,
            ShadowWidth,
            ShadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,
                TextureManager::Get()->GetTextures()[0].textureID);
            RenderScene(*simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);
        glViewport(0, 0, Width, Height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderNormalScene(*lightShader, lightSpaceMatrix);

        /*
        debugDepthMapQuad->UseShaderProgram();
        debugDepthMapQuad->SetFloat("near_plane", light_near_plane);
        debugDepthMapQuad->SetFloat("far_plane", light_far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);*/
        /*renderQuad();*/


        lightShader->UseShaderProgram();

        IMGUIClass::InitUpdateLoop();

        IMGUIClass::MenuBar();

        IMGUIClass::CameraWindow(*camera);

        IMGUIClass::EntityWindow(*lightShader);

        IMGUIClass::ModelsWindow();

        IMGUIClass::TexturesWindow();

        IMGUIClass::LightsWindow(*lightShader);

        IMGUIClass::EndUpdateLoop();


        glfwSwapBuffers(window);
        glfwPollEvents();
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
            i->CombineTransformMatrix();
            shader.SetMatrix4("model", i->transform);
            Mesh m = MeshManager::Instance()->FindMesh(i->meshName);
            virtual_object->SetVertexBufferObjects(m);
            virtual_object->Draw(virtual_object->VAO);
            /*virtual_object->RenderCube();*/
        }
    }

    void GameEngine::RenderNormalScene(Shader &shader, glm::mat4& lightSpaceMatrix) {
        shader.UseShaderProgram();
        glm::mat4 projection;


        /*projection = glm::ortho(-10.0f, 10.0f,
          -10.0f, 10.0f,
        light_near_plane, light_far_plane);*/

        //Perspective
        projection = glm::perspective(glm::radians(45.0f),
            static_cast<float>(Width) / static_cast<float>(Height),
            0.1f, 100.0f);

        glm::mat4 view {glm::mat4(1.0f)};
        camera->MoveCamera(Input::GetMoveValue(), 0.1, &view);

        LightEntityManager::Get().SetLightsUniforms(shader);
        shader.SetMatrix4("view", view);
        shader.SetMatrix4("projection", projection);

        shader.SetVector3("viewPos", camera->GetCameraPos());
        shader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

        shader.SetInt("nr_pointlights",
                    LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::PointLight));
        shader.SetInt("nr_dirlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::DirectionalLight));
        shader.SetInt("nr_spotlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::SpotLight));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Get()->GetTextures()[0].textureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Get()->GetTextures()[1].textureID);


        RenderScene(shader);
    }






}
