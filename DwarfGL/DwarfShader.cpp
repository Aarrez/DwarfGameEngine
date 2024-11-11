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


Dwarf::DwarfShader::~DwarfShader(){

}


