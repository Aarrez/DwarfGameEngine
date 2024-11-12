#include "DwarfEngine.h"
#include "../DwarfGL/Mesh/DwarfMesh.h"

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

   /* mesh2D = new Mesh2D::Triangle(3, 3);*/
    mesh2D = new Mesh2D::Square(3, 6);

}

void DwarfEngine::Update() {
    glfwPollEvents();
}

void DwarfEngine::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(70.f/255.f, 116.f/255.f, 93.f/255.f, 1.0f);
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



