#include "Graphics/texture.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace gmh;

Texture::Texture(const char* filepath, TextureType texturetype): path(filepath), type(texturetype) {
    stbi_set_flip_vertically_on_load(1);
    buffer = std::shared_ptr<unsigned char>(stbi_load(filepath, &width, &height, &BPP, 4), std::free);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const Texture& tex): buffer(tex.buffer), path(tex.path), width(tex.width), height(tex.height), BPP(tex.BPP), type(tex.type) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& tex): path(std::move(tex.path)), width(tex.width), height(tex.height), BPP(tex.BPP), type(tex.type) {
    id = tex.id;
    buffer = tex.buffer;
    tex.id = 0;
    tex.buffer = nullptr;
}

Texture::~Texture(){
    glDeleteTextures(1, &id);
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
    type = tex.type;
    buffer = tex.buffer;
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return *this;
}

Texture& Texture::operator=(Texture&& tex){
    glDeleteTextures(1, &id);
    path = std::move(tex.path);
    id = tex.id;
    buffer = tex.buffer;
    tex.id = 0;
    tex.buffer = nullptr;
    width = tex.width;
    height = tex.height;
    BPP = tex.BPP;
    type = tex.type;
    return *this;
}

const std::string Texture::typeNames[NUMTEXTYPES] = {"texture_none", "texture_diffuse", "texture_specular", "texture_ambient", "texture_emissive", "texture_height", "texture_normals"};
