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
  class TextureManager {

    static TextureManager* instance;
    static std::vector<std::string> imagePathList;
    std::vector<Texture> textures;
    static std::string defaultTexturePath;
  public:
    static void Allocate();
    static TextureManager* Instance();
    std::vector<Texture>& GetTextures();
    void FillTextureList();
    void GenerateTextures();
    void DrawTexture(const Texture& texture);

  };
}




#endif //TEXTUREMANAGER_H
