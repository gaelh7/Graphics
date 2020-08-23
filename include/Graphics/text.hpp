#pragma once

#include <unordered_map>
#include <string>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float3.hpp>
#include "Graphics/shader.hpp"
#include "Graphics/window.hpp"

namespace gmh {
    struct Character {
        unsigned int id;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    class Font {
        static Window* window;
        static Shader s;
        static unsigned int VAO, VBO;
        std::unordered_map<char, Character> characters;
        public:
            static void init(Window* win);
            static void terminate();
            static void bind();
            static void unbind();
            Font(const char* path, unsigned int font_size);
            void render(std::string text, float x, float y, float scale, glm::vec3 color) const;
    };
}
