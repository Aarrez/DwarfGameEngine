#include "DwarfGetImage.h"

unsigned char *Dwarf::Image::DwarfImage::GetImage(
        const char *image_name, int &width, int &height, int &nrChannel) {
    auto path = Dwarf::File::DwarfPathChange::GetCurrentPath();
    Dwarf::File::DwarfPathChange::AppendCurrentPath("DwarfImages");
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannel, 0);
    Dwarf::File::DwarfPathChange::SetCurrentPath(path);
    return data;
}