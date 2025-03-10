#define GLAD_GL_IMPLEMENTATION
#include <glad/glad.h>
#include "Shader.h"
#include <iostream>
#include "ShaderProgramCode.h"

namespace Engine {

  Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
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

  void Shader::SetBool(const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), (int)value);
  }
  void Shader::SetInt(const char*  name, int value) {
    glUniform1i(glGetUniformLocation(shaderProgram, name), value);
  }
  void Shader::SetFloat(const char*  name, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram, name), value);
  }
  void Shader::SetVector3(const char* name, float value1, float value2, float value3) {
    glUniform3f(glGetUniformLocation(shaderProgram, name), value1, value2, value3);
  }
  void Shader::SetVector3(const char* name, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(shaderProgram, name), value.x, value.y, value.z);
  }
  void Shader::SetVector4(const char* name,
                                      float value1, float value2, float value3, float value4) {
    glUniform4f(glGetUniformLocation(shaderProgram,
                                     name), value1, value2, value3, value4);
  }
  void Shader::SetMatrix4(const char *name, glm::mat4 &value) {
    GLint transformLoc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
  }

}

