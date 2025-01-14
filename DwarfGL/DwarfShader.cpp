#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include "DwarfShader.h"
#include <iostream>
#include "ShaderProgramCode.h"


Engine::DwarfShader::DwarfShader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    int result;
    char Log[512];

    vertexShader = DwarfGetShader::LoadVertexShader(vertexShaderPath);
    fragmentShader = DwarfGetShader::LoadFragmentShader(fragmentShaderPath);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);

    if(!result){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, Log);
        std::cerr << "Failed to compile Shader Program\n" << Log << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Engine::DwarfShader::SetBool(const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), (int)value);
}
void Engine::DwarfShader::SetInt(const char*  name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}
void Engine::DwarfShader::SetFloat(const char*  name, float value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}
void Engine::DwarfShader::SetVector3(const char* name,
                                    float value1, float value2, float value3) {
    glUniform3f(glGetUniformLocation(shaderProgram,
                                     name), value1, value2, value3);
}
void Engine::DwarfShader::SetVector4(const char* name,
                                    float value1, float value2, float value3, float value4) {
    glUniform4f(glGetUniformLocation(shaderProgram,
                                     name), value1, value2, value3, value4);
}

void Engine::DwarfShader::SetMatrix4(const char *name,
                                    int num_matrix, int transpose, glm::mat4 &value) {
    GLint transformLoc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(transformLoc, num_matrix, transpose, glm::value_ptr(value));

}




