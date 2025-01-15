#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <vector>
#include <cassert>
#include <iostream>


namespace Engine {
  class TextureManager {

    static TextureManager* instance;
    static std::vector<std::string> imagePathList;
  public:
    static void Allocate();
    static TextureManager* Instance();
    void AddImage(const std::string& imagePath);



  };
}




#endif //TEXTUREMANAGER_H
