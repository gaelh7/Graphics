#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include "Graphics/texture.hpp"

using namespace gmh;

Texture::Texture(const char* filepath): path(filepath){
    stbi_set_flip_vertically_on_load(1);
    buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const Texture& tex): path(tex.path), width(tex.width), height(tex.height), BPP(tex.BPP) {
    buffer = reinterpret_cast<unsigned char*>(malloc(width*height*4));
    for(unsigned int i = 0; i < width*height*4; i++) buffer[i] = tex.buffer[i];
    std::cout << width*height << '\n';
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& tex): path(std::move(tex.path)), width(tex.width), height(tex.height), BPP(tex.BPP) {
    id = tex.id;
    buffer = tex.buffer;
    tex.id = 0;
    tex.buffer = nullptr;
}

Texture::~Texture(){
    glDeleteTextures(1, &id);
    free(buffer);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture& Texture::operator=(const Texture& tex){
    path = tex.path;
    width = tex.width;
    height = tex.height;
    BPP = tex.BPP;
    buffer = reinterpret_cast<unsigned char*>(malloc(width*height*4));
    for(unsigned int i = 0; i < width*height*4; i++) buffer[i] = tex.buffer[i];
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return *this;
}

Texture& Texture::operator=(Texture&& tex){
    glDeleteTextures(1, &id);
    free(buffer);
    path = std::move(tex.path);
    id = tex.id;
    buffer = tex.buffer;
    tex.id = 0;
    tex.buffer = nullptr;
    width = tex.width;
    height = tex.height;
    BPP = tex.BPP;
    return *this;
}
