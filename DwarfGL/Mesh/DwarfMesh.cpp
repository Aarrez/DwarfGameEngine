#include "DwarfMesh.h"

namespace Dwarf{

    DwarfMesh::DwarfMesh(std::vector<ModelVertex> vertices,
                                 std::vector<unsigned int> indices,
                                 std::vector<ModelTexture> texture)
                                 : vertices(vertices), indices(indices), textures(texture){
        setupMesh();
    }

    void DwarfMesh::Draw(DwarfShader* shader) {
        unsigned int diffuseNr = 1;
        unsigned int secularNr = 1;
        for(unsigned int i = 0; i < textures.size(); i++){
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(secularNr++);

            shader->SetInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void DwarfMesh::setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (ModelVertex),
                     &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, TexCoords));

        glBindVertexArray(0);


    }

    void DwarfModel::Draw(DwarfShader* shader) {
        for(auto & mesh : meshes){
            mesh.Draw(shader);
        }
    }

    void DwarfModel::loadModel(const std::string &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path,
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_GenSmoothNormals |
                                                 aiProcess_CalcTangentSpace);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void DwarfModel::processNode(aiNode *node, const aiScene *scene) {
        for(unsigned int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for(unsigned int i = 0; node->mNumChildren; i++){
            processNode(node->mChildren[i], scene);
        }
    }

    DwarfMesh DwarfModel::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<ModelVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<ModelTexture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++){
            ModelVertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            vertices.push_back(vertex);
            if(mesh->mTextureCoords[0]){
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        for(unsigned int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j< face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        if(mesh->mMaterialIndex >= 0){
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<ModelTexture> diffuseMaps = loadMaterialTextures(material,
                                                                         aiTextureType_DIFFUSE,
                                                                         "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<ModelTexture> specularMaps = loadMaterialTextures(material,
                                                                          aiTextureType_SPECULAR,
                                                                          "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return DwarfMesh(vertices, indices, textures);
    }

    std::vector<ModelTexture> DwarfModel::loadMaterialTextures(aiMaterial *mat,
                                                               aiTextureType type,
                                                               std::string typeName) {
        std::vector<ModelTexture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(auto & j : textures_loaded){
                if(std::strcmp(j.path.data(), str.C_Str()) == 0){
                    textures.push_back(j);
                    skip = true;
                    break;
                }
            }
            if(!skip){
                ModelTexture texture;
                if(directory.empty())
                    texture.id = TextureFromFile(str.C_Str());
                else
                    texture.id = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }
}


