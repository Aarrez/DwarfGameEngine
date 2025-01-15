#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <vector>


namespace Engine {
  class TextureManager {

    static TextureManager* instance;
    static std::vector<std::string> textureList;
  public:
    static void Allocate();
    static TextureManager* Instance();



  };
}




#endif //TEXTUREMANAGER_H
