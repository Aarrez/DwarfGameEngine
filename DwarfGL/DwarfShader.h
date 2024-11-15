#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

        //Set Matrix variables
        void SetMatrix4(const char* name,
                        int num_matrix, int transpose, glm::mat4 &trans);

        GLuint shaderProgram;
        //Shaders
    private:
        GLuint vertexShader{};
        GLuint fragmentShader{};
    };
}






