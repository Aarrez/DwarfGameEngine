#pragma once
#include <string>

namespace Dwarf {



    class DwarfShader {
    public:
        DwarfShader();

        void UseShaderProgram(){
            glUseProgram(shaderProgram);
        }
        GLuint* GetVertexShader(){
            return &vertexShader;
        }
        GLuint* GetFragmentShader(){
            return &fragmentShader;
        }



        //Set misc variables
        void SetBool(const char*  name, bool value);
        void SetInt(const char* name, int value);
        void SetFloat(const char* name, float value);

        //Set vector variable
        void SetVector3(const char* name,
                        float value1, float value2, float value3);
        void SetVector4(const char* name,
                        float value1, float value2, float value3, float value4);

        GLuint shaderProgram;
        //Shaders
    private:
        GLuint vertexShader{};
        GLuint fragmentShader{};
    };
}






