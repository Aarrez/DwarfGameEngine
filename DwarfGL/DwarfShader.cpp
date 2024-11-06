//
// Created by Aaron on 2024-11-05.
//
#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include "DwarfShader.h"

#include <iostream>

#include "ShaderProgramCode.h"


Dwarf::DwarfShader::DwarfShader() {
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto VShader = DwarfGetShader::GetVertexShader();
    glShaderSource(vertexShader, 1, &VShader, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto FShader = DwarfGetShader::GetFragmentShader();
    glShaderSource(fragmentShader, 1, &FShader, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

Dwarf::DwarfShader::~DwarfShader() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Dwarf::DwarfShader::PrepareTriangle(Vertex *vertices, unsigned int numVertices) {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, numVertices, GL_FLOAT, GL_FALSE, numVertices * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


void Dwarf::DwarfShader::PrepareQuad(Vertex* vertices, unsigned int numVertices) {

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeices), indeices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, numVertices, GL_FLOAT, GL_FALSE, numVertices * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


