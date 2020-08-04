#pragma once
#include <map>
#include <string>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float3.hpp>
#include "Graphics/shader.hpp"

namespace gmh {
    struct Character {
        unsigned int id;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    class Font {
        static Shader s;
        static unsigned int VAO, VBO;
        std::map<char, Character> characters;
        public:
            static void init();
            static void terminate();
            static void unbind();
            Font(std::string path, unsigned int font_size);
            void bind() const;
            void render(std::string text, float x, float y, float scale, glm::vec3 color) const;
    };
}
