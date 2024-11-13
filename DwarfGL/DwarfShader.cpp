#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include "DwarfShader.h"
#include <iostream>
#include "ShaderProgramCode.h"


Dwarf::DwarfShader::DwarfShader() {
    int result;
    char Log[512];

    vertexShader = DwarfGetShader::LoadVertexShader();
    fragmentShader = DwarfGetShader::LoadFragmentShader();

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

void Dwarf::DwarfShader::SetBool(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}
void Dwarf::DwarfShader::SetInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void Dwarf::DwarfShader::SetFloat(const std::string &name, float value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}
void Dwarf::DwarfShader::SetVector3(const std::string &name,
                                    float value1, float value2, float value3) {
    glUniform3f(glGetUniformLocation(shaderProgram,
                                     name.c_str()), value1, value2, value3);
}
void Dwarf::DwarfShader::SetVector4(const std::string &name,
                                    float value1, float value2, float value3, float value4) {
    glUniform4f(glGetUniformLocation(shaderProgram,
                                     name.c_str()), value1, value2, value3, value4);
}




