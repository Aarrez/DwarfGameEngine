#include "ShaderProgramCode.h"


using namespace Dwarf;

std::string DwarfGetShader::ReadShaderFile(const char *pathToFile) {
    std::ifstream fileStream(pathToFile, std::ios::in);
    std::string content;
    if(!fileStream.is_open()){
        std::cerr << "Could not read file" << pathToFile << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint DwarfGetShader::LoadFragmentShader(const char* path) {

    int result;
    char Log[512];
    auto shaderString = ReadShaderFile(path);
    if(shaderString.empty()) return 0;

    auto Fragment = shaderString.c_str();

    GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &Fragment, nullptr);
    glCompileShader(FragmentShader);

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);

    if(!result){
        glGetShaderInfoLog(FragmentShader, 512, nullptr,  Log);
        std::cerr << "Failed to compile Fragment Shader \n" << Log << std::endl;
    }

    return FragmentShader;
}

GLuint DwarfGetShader::LoadVertexShader(const char* path) {

    int result;
    char Log[512];

    auto shaderString = ReadShaderFile(path);
    auto Vertex = shaderString.c_str();

    GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &Vertex, nullptr);
    glCompileShader(VertexShader);

    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);

    if(!result){
        glGetShaderInfoLog(VertexShader, 512, nullptr, Log);
        std::cerr << "Failed to compile ModelVertex Shader \n" << Log << std::endl;
    }

    return VertexShader;
}