#pragma once

#include <string>

namespace gmh{
    class Texture {
        private:
            unsigned int id;
            std::string path;
            unsigned char* buffer;
            int width, height, BPP;
        public:
            Texture(const char* filepath);
            Texture(const Texture& tex);
            Texture(Texture&& tex);
            ~Texture();
            void bind(unsigned int slot = 0) const;
            Texture& operator=(const Texture& tex);
            Texture& operator=(Texture&& tex);
    };
}
