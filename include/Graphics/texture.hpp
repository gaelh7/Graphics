#pragma once

class Texture {
    private:
        unsigned int id;
        const char* path;
        unsigned char* buffer;
        int width, height, BPP;
    public:
        Texture(const char* filepath);
        ~Texture();
        void bind(unsigned int slot = 0) const;
};
