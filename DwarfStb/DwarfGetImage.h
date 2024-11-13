#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include "../DwarfMisc/DwarfPath.h."

namespace Dwarf::Image{
    class DwarfImage{

        static unsigned char* GetImage(const char* image_name, int &width, int &height, int &nrChannel);
    };
}