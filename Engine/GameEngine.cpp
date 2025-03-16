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
        /*glfwSwapInterval(1);*/

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
            return;
        }
        glEnable(GL_DEPTH_TEST);


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

        IMGUIClass::InitImGui(window);
    }

    void GameEngine::Init() {

        /*mainShader = std::make_shared<Shader>(
            "ShaderScripts/Misc/VertexShader.vert",
            "ShaderScripts/Misc/FragmentShader.frag");*/
        lightShader = std::make_shared<Shader>(
            "ShaderScripts/Light/LightingVertexShader.vert",
            "ShaderScripts/Light/LightingFragmentShader.frag");
        simpleDepthShader = std::make_shared<Shader>(
            "ShaderScripts/Shadow/SimpleDepthShader.vert",
            "ShaderScripts/Shadow/SimpleDepthShader.frag");
        debugDepthMapQuad = std::make_shared<Shader>(
        "ShaderScripts/Shadow/debug_quad.vert",
        "ShaderScripts/Shadow/debug_quad_depth.frag");


        ShadowManager::Get()->GenerateDepthMap(&shadow_map);
        TextureManager::Get()->StbGenerateTextures();
        camera = std::make_unique<Camera>();
        Input::SetCameraRef(camera.get());
        std::string s = "TheCube.bin";
        cube_mesh = MeshManager::Instance()->FindMesh(s);
        virtual_object = make_unique<VirtualObject>(mainShader, cube_mesh);
        virtual_object->BindLightVAO();

        EntityMessage entMsg(MessageType::CreateEntity, "Entity");
        entMsg.file = OBJLoader::FilesSerialized[2];
        entMsg.texture = TextureManager::Get()->GetTextures()[0];
        entMsg.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg);

        EntityMessage entMsg2(MessageType::CreateEntity, "Entity");
        entMsg2.file = OBJLoader::FilesSerialized[6];
        entMsg2.texture = TextureManager::Get()->GetTextures()[0];
        entMsg2.spec_texture = TextureManager::Get()->GetTextures()[1];
        EntityManager::Get().ProcessMessages(entMsg2);

        auto* entlist = &EntityManager::Get().GetEntityList();

        for(int i = 0; i < entlist->size(); i++) {
            entlist->at(i)->Translate({0, -2.5 * i, 0});
            if (i == 1)
                entlist->at(i)->SetScale({10, .2, 10});
        }

        LightTypes type {LightTypes::PointLight};
        auto ent = LightEntityManager::Get().CreateLight(type);
        ent->SetPosition({0.0f, 3.0f, -1.0f});
        ent->CombineModels();


        lightShader->UseShaderProgram();
        lightShader->SetInt("diffuseTexture", 0);
        /*lightShader->SetInt("material.specular", 1);*/
        lightShader->SetInt("shadowMap", 1);


        debugDepthMapQuad->UseShaderProgram();
        debugDepthMapQuad->SetInt("depthMap", 0);
    }

    void GameEngine::Update() {

        glfwPollEvents();
    }

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad() {
        if (quadVAO == 0) {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };

            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER,
                sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3,
                GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2,
                GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    void GameEngine::Render() {

        glfwGetFramebufferSize(window, &Width, &Height);

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 lightProjection;
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
        glm::mat4 lightSpaceMatrix;
        glm::mat4 lightView = glm::lookAt(
          lightPos,
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightView * lightProjection;
        simpleDepthShader->UseShaderProgram();
        simpleDepthShader->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0,
            static_cast<int>(shadow_map.ShadowWidth),
            static_cast<int>(shadow_map.ShadowHeight));
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_map.depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,
                TextureManager::Get()->GetTextures()[0].textureID);

            RenderScene(*simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, Width, Height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*RenderNormalScene(*lightShader, lightSpaceMatrix);*/

        debugDepthMapQuad->UseShaderProgram();
        debugDepthMapQuad->SetFloat("near_plane", light_near_plane);
        debugDepthMapQuad->SetFloat("far_plane", light_far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadow_map.depthMap);
        renderQuad();
        /*
        mainShader->UseShaderProgram();*/

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
            /*virtual_object->SetVertexBufferObjects(m);

            virtual_object->Draw(virtual_object->VAO);*/
            virtual_object->RenderCube();
        }

    }

    /*void GameEngine::TempRenderScene(Shader &shader) {
        glm::mat4 model = glm::mat4(1.0f);
        shader.SetMatrix4("model", model);
        virtual_object->RenderPlane();
        for (auto& i : EntityManager::Get().GetEntityList()) {
            i->SetScale(glm::vec3(0.5f));
            i->SetPostion(glm::vec3(0, 2, 0));
            i->CombineTransformMatrix();
            shader.SetMatrix4("model", i->transform);
            virtual_object->RenderCube();
        }
    }*/

    void GameEngine::RenderNormalScene(Shader &shader, glm::mat4& lightSpaceMatrix) {
        shader.UseShaderProgram();
        glm::mat4 projection;


        projection = glm::ortho(-10.0f, 10.0f,
          -10.0f, 10.0f,
        light_near_plane, light_far_plane);

        //Perspective
        /*projection = glm::perspective(glm::radians(45.0f),
            static_cast<float>(Width) / static_cast<float>(Height),
            0.1f, 100.0f);*/

        glm::mat4 view {glm::mat4(1.0f)};
        view = glm::lookAt(
            lightPos,
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
            );
        /*camera->MoveCamera(Input::GetMoveValue(), 0.1, &view);*/

        /*TextureManager::Get()->SetTextureUniform(shader);*/
        LightEntityManager::Get().SetLightsUniforms(shader);
        shader.SetMatrix4("view", view);
        shader.SetMatrix4("projection", projection);

        shader.SetVector3("viewPos", camera->GetCameraPos());
        shader.SetVector3("lightPos",
            lightPos);
        shader.SetMatrix4("lightSpaceMatrix", lightSpaceMatrix);

        /*shader.SetInt("nr_pointlights",
                    LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::PointLight));
        shader.SetInt("nr_dirlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::DirectionalLight));
        shader.SetInt("nr_spotlights",
            LightEntityManager::Get().GetNumberOfLightsOfType(LightTypes::SpotLight));*/

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Get()->GetTextures()[0].textureID);
        /*glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TextureManager::Get()->GetTextures()[1].textureID);*/
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadow_map.depthMap);

        RenderScene(shader);
    }






}
