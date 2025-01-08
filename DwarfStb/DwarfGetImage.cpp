#include "DwarfGetImage.h"

unsigned char *Dwarf::DwarfImage::GetImage(
        const char* image_name, int &width, int &height, int &nrChannel) {
    auto path = DwarfPathChange::GetCurrentPath();
    DwarfPathChange::AppendCurrentPath("DwarfImages");
    unsigned char* data = stbi_load(image_name, &width, &height, &nrChannel, 0);
    DwarfPathChange::SetCurrentPath(path);
    return data;
}

unsigned int Dwarf::TextureFromFile(const char *filename) {

    auto p = DwarfPathChange::GetCurrentPath();
    DwarfPathChange::AppendCurrentPath("DwarfImages");
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
    if(data){
        GLenum format;
        if(nrComponents == 1){
            format = GL_RED;
        }else if(nrComponents == 3){
            format = GL_RGB;
        }else if(nrComponents == 4){
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0,
                     format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        std::cerr << "Failed to load ModelTexture: " << filename << std::endl;
    }
    DwarfPathChange::SetCurrentPath(p);
    return textureID;
}

unsigned int Dwarf::TextureFromFile(const std::string &filename, const std::string &directory) {
    auto p = DwarfPathChange::GetCurrentPath();
    DwarfPathChange::SetCurrentPath(directory);
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if(data){
        GLenum format;
        if(nrComponents == 1){
            format = GL_RED;
        }else if(nrComponents == 3){
            format = GL_RGB;
        }else if(nrComponents == 4){
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0,
                     format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        std::cerr << "Failed to load ModelTexture: " << filename << std::endl;
    }
    DwarfPathChange::SetCurrentPath(p);
    return textureID;
}
