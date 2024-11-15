#include "DwarfEngine.h"
#include <memory>



using namespace Dwarf;

DwarfEngine::DwarfEngine(): window(nullptr) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    File::DwarfPathChange::ChangeCurrentPathToProjectRoot();
    window = glfwCreateWindow(800, 600, "DwarfEngine", nullptr, nullptr);
    if (window == nullptr) {
        /*std::cerr << "Failed to create GLFW window" << std::endl;*/
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}



void DwarfEngine::Init() {

    shader = new DwarfShader();
    dwarfTransform = new Transform::DwarfTransform(shader);

    /*mesh2D = new Mesh2D::Triangle(3, 6);*/
    mesh2D = new Mesh2D::Square(shader);

    mesh2D->SetTextureUnit();

    transform = glm::mat4(1.0f);

    dwarfTransform->ScaleObject(transform, glm::vec3(.5f, .5, .5f));


}

void DwarfEngine::Update() {
    glfwPollEvents();
}

void DwarfEngine::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f/255.f, 0.0f/255.f, 0.0f/255.f, 1.0f);

    mesh2D->BindOnTextureUnit();

    dwarfTransform->RotateObject(transform, sin(float(glfwGetTime())), glm::vec3(0.0, 0.0, 1.0));
    dwarfTransform->ApplyTransformChanges(transform);

    shader->UseShaderProgram();
    /*mesh_2d->Draw(shader);*/
    mesh2D->Draw(shader);

    glfwSwapBuffers(window);

}

void DwarfEngine::Shutdown() {

}

DwarfEngine::~DwarfEngine() {
    delete mesh2D;
    delete shader;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void DwarfEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}



