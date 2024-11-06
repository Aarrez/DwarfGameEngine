#include "ShaderProgramCode.h"

using namespace Dwarf;

void DwarfGetShader::readVertexFile(const GLchar *pathToFile) {
    std::ifstream fileStream(pathToFile, std::ios::in);

    if(!fileStream.is_open()){
        std::cerr << "Could not read file" << pathToFile << ". File does not exist." << std::endl;
        return;
    }
    VertexShader = "";
    std::string line = "";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        VertexShader.append(line + "\n");
    }
    fileStream.close();
    std::cout << "'" << VertexShader << "'" << std::endl;
}

void DwarfGetShader::readFragmentFile(const GLchar *pathToFile) {
    std::ifstream fileStream(pathToFile, std::ios::in);

    if(!fileStream.is_open()){
        std::cerr << "Could not read file" << pathToFile << ". File does not exist." << std::endl;
        return;
    }
    FragmentShader = "";
    std::string line = "";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        FragmentShader.append(line + "\n");
    }
    fileStream.close();
    std::cout << "'" << FragmentShader << "'" << std::endl;
}

const GLchar* DwarfGetShader::GetVertexShader() {
    readVertexFile("ShaderScripts/VertexShader.glsl");
    return VertexShader.c_str();
}

const GLchar* DwarfGetShader::GetFragmentShader() {
    readFragmentFile("ShaderScripts/FragmentShader.glsl");
    return FragmentShader.c_str();
}
