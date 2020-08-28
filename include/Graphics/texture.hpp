#pragma once

#include <string>

namespace gmh {
    enum TextureType {
        TEXTYPENONE, DIFFUSE, SPECULAR, AMBIENT, EMISSIVE, HEIGHT, NORMALS, NUMTEXTYPES
    };

    class Texture {
        private:
            unsigned int id;
            unsigned char* buffer;
            int width, height, BPP;
        public:
            std::string path;
            static const std::string typeNames[NUMTEXTYPES];
            TextureType type;
            Texture(const char* filepath, TextureType texturetype = TEXTYPENONE);
            Texture(const Texture& tex);
            Texture(Texture&& tex);
            ~Texture();
            void bind(unsigned int slot = 0) const;
            Texture& operator=(const Texture& tex);
            Texture& operator=(Texture&& tex);
    };
}
