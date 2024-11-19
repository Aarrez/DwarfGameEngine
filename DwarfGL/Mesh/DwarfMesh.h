#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "stb_image.h"
#include "../DwarfShader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "../DwarfStb/DwarfGetImage.h"



namespace Dwarf::Mesh {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture{
        unsigned int id;
        std::string type;
        std::string path;
    };

    class DwarfMesh{
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        DwarfMesh(std::vector<Vertex> vertices,
                  std::vector<unsigned int> indices,
                  std::vector<Texture> texture);

        void Draw(DwarfShader* shader);

    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        void setupMesh();
    };

    class DwarfModel{
    public:
        DwarfModel(char* path){
            loadModel(path);
        }
        void Draw(DwarfShader* shader);
    private:
        std::vector<DwarfMesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;

        void loadModel(const std::string &path);
        void processNode(aiNode *node, const aiScene *scene);
        DwarfMesh processMesh(aiMesh *mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                                  std::string typeName);
    };




}


