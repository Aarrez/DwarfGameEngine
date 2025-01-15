#include "TextureManager.h"

#include "../DwarfMisc/DwarfPath.h"


namespace Engine {
  TextureManager* TextureManager::instance = nullptr;
  std::vector<std::string> TextureManager::imagePathList;

  void TextureManager::Allocate() {
    assert(instance == nullptr);
    if (instance != nullptr)
      return;
    instance = new TextureManager();
    std::string s = "Images/";
    auto list = DwarfPathChange::GetNameFilesInDirectory(s);
    imagePathList = list;
  }

  TextureManager *TextureManager::Instance() {
    return instance;
  }

  void TextureManager::AddImage(const std::string& imagePath) {
    for (auto s : imagePathList) {
      if (s == imagePath) {
        std::cout << "Image with path \""<< imagePath <<"\" already exists"<< std::endl;
        return;
      }
    }
    imagePathList.push_back(imagePath);
  }
}
