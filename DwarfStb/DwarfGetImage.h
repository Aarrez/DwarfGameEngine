#pragma once
#include "glad/glad.h"
#include "stb_image.h"
#include <string>
#include "../DwarfMisc/DwarfPath.h"


namespace Dwarf::Image{
    class DwarfImage{
    public:
        static unsigned char* GetImage(const char* image_name, int &width, int &height, int &nrChannel);
    };
}

namespace Dwarf::Texture{
    unsigned int TextureFromFile(const char* filename);

    unsigned int TextureFromFile(const std::string &filename, const std::string &directory);
}