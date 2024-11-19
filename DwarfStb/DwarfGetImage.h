#pragma once
#include "glad/glad.h"
#include "stb_image.h"
#include <string>
#include "../DwarfMisc/DwarfPath.h."


namespace Dwarf::Image{
    class DwarfImage{
    public:
        static unsigned char* GetImage(const char* image_name, int &width, int &height, int &nrChannel);
    };
}

namespace Dwarf::Texture{
    unsigned int TextureFromFile(const char* filename){
        auto p = Dwarf::File::DwarfPathChange::GetCurrentPath();
        Dwarf::File::DwarfPathChange::AppendCurrentPath("DwarfImages");
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
            std::cerr << "Failed to load Texture: " << filename << std::endl;
        }
        Dwarf::File::DwarfPathChange::SetCurrentPath(p);
        return textureID;
    }

    unsigned int TextureFromFile(const std::string &filename, const std::string &directory){
        using namespace Dwarf::File;
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
            std::cerr << "Failed to load Texture: " << filename << std::endl;
        }
        DwarfPathChange::SetCurrentPath(p);
        return textureID;
    }
}