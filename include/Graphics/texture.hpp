#pragma once

#include <string>
#include "glad/glad.h"
#include "Graphics/log.hpp"

class Texture {
    private:
        unsigned int id;
        std::string path;
        unsigned char* buffer;
        int width, height, BPP;
    public:
        Texture(const char* filepath);
        ~Texture();
        void bind(unsigned int slot = 0) const;
};
