#pragma once

#include <string>
#include <memory>

namespace gmh {
    enum TextureType {
        TEXTYPENONE, DIFFUSE, SPECULAR, AMBIENT, EMISSIVE, HEIGHT, NORMALS, NUMTEXTYPES
    };

    class Texture {
        static const std::string typeNames[NUMTEXTYPES];
        std::string path;
        unsigned int id;
        std::shared_ptr<unsigned char> buffer;
        int width, height, BPP;
        TextureType type;
        public:
            Texture(const char* filepath, TextureType texturetype = TEXTYPENONE);
            Texture(const Texture& tex);
            Texture(Texture&& tex);
            ~Texture();
            void bind(unsigned int slot = 0) const;
            Texture& operator=(const Texture& tex);
            Texture& operator=(Texture&& tex);

        friend class Model;
        friend class Mesh;
    };
}
