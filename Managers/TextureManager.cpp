#include "TextureManager.h"

#include "../GL/Shader.h"


namespace Engine {
  TextureManager* TextureManager::instance = nullptr;
  std::vector<std::string> TextureManager::imagePathList;
  std::string TextureManager::defaultTexturePath = "Images/";

  void TextureManager::Allocate() {
    assert(instance == nullptr);
    if (instance != nullptr)
      return;
    instance = new TextureManager();
    auto list = DwarfPathChange::GetNameFilesInDirectory(defaultTexturePath);
    imagePathList = list;
    instance->FillTextureList();
  }

  TextureManager *TextureManager::Get() {
    return instance;
  }

  std::vector<Texture>& TextureManager::GetTextures() {
    return textures;
  }

  void TextureManager::FillTextureList() {
    for (auto& path : imagePathList) {
      Texture tempTexture{};
      tempTexture.colorFormat = GL_RGBA;
      tempTexture.filePath = "Images/" + path;
      tempTexture.fileName = path;
      tempTexture.textureID = 0;
      textures.push_back(tempTexture);
    }
  }

  void TextureManager::StbGenerateTextures() {
    for (auto& tex : textures) {
      glGenTextures(1, &tex.textureID);
      glBindTexture(GL_TEXTURE_2D, tex.textureID);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


      int width, height, nrChannels;
      unsigned char* data =
        stbi_load(tex.filePath.c_str(),
          &width, &height, &nrChannels, 0);
      if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
          width, height, 0, tex.colorFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      else {
        std::cout << "Failed to load texture" << std::endl;
      }
      stbi_image_free(data);
    }
  }

  void TextureManager::DrawTexture(const Texture &texture) {
    glActiveTexture(GL_TEXTURE0 + texture.textureID);
    glBindTexture(GL_TEXTURE_2D, texture.textureID);
  }

  void TextureManager::SetTextureUniform(Shader& shader) {
    shader.UseShaderProgram();
    shader.SetInt("material.diffuse", 1);
    shader.SetInt("material.specular", 2);
  }
}
