#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include "../Misc/Path.h"
#include "../Misc/PublicStructs.h"


namespace Engine {
  class TextureManager {

    static TextureManager* instance;
    static std::vector<std::string> imagePathList;
    std::vector<Texture> textures;
  public:
    static void Allocate();
    static TextureManager* Instance();
    void AddImage(const std::string& imagePath);
    void LoadTexture(const std::string& imagePath);



  };
}




#endif //TEXTUREMANAGER_H
