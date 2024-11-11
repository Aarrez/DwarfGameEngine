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
    shader = std::make_unique<DwarfShader>();
    glGenBuffers(1, &shader->vertex_buffer_object);
    glGenVertexArrays(1, &shader->vertex_array_object);
    glBindVertexArray(shader->vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, shader->vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, shader->vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void DwarfEngine::Update() {

}

void DwarfEngine::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(70.f/255.f, 116.f/255.f, 93.f/255.f, 1.0f);
    glUseProgram(shader->shaderProgram);
    glBindVertexArray(shader->vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void DwarfEngine::Shutdown() {

}

DwarfEngine::~DwarfEngine() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void DwarfEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}



