#include "Graphics/texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* filepath): id(0), path(filepath), buffer(nullptr), width(0), height(0), BPP(0) {
    stbi_set_flip_vertically_on_load(1);
    buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
    GLCALL(glGenTextures(1, &id));
    GLCALL(glBindTexture(GL_TEXTURE_2D, id));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if(buffer)
        stbi_image_free(buffer);
}

Texture::~Texture(){
    glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int slot) const{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
    GLCALL(glBindTexture(GL_TEXTURE_2D, id));
}
