#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "../Misc/PublicStructs.h"
#include <glad/glad.h>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <stb_image.h>
#include "../Misc/Path.h"

namespace Engine {
  class Shader;

  class TextureManager {

    static TextureManager* instance;
    static std::vector<std::string> imagePathList;
    std::vector<Texture> textures;
    static std::string defaultTexturePath;
  public:
    static void Allocate();
    static TextureManager* Get();
    std::vector<Texture>& GetTextures();
    void FillTextureList();
    void StbGenerateTextures();
    void DrawTexture(const Texture& texture);
    void SetTextureUniform(Shader& shader);
    void ChangeMipMapSettings(Texture& texture);
  };
}
#endif //TEXTUREMANAGER_H
