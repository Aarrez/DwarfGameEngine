#pragma once

#include "stb_image.h"
#include <string>
#include "../DwarfMisc/DwarfPath.h."


namespace Dwarf::Image{
    class DwarfImage{
    public:
        static unsigned char* GetImage(const char* image_name, int &width, int &height, int &nrChannel);
    };
}