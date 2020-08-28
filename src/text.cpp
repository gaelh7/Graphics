#include "Graphics/text.hpp"
#include <iostream>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/type_ptr.hpp>

using namespace gmh;

Window* Font::window = nullptr;
Shader Font::program = Shader();
unsigned int Font::VAO = 0;
unsigned int Font::VBO = 0;

void Font::init(Window* win){
    window = win;
    program = Shader(PROJECT_DIR "/res/shaders/text.glsl");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), reinterpret_cast<void*>(2*sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::terminate(){
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Font::bind(){
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    program.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
}

void Font::unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

Font::Font(const char* path, unsigned int font_size){
    FT_Library lib;
    FT_Face face;
    if(FT_Init_FreeType(&lib)){
        std::cerr << "Failed to initialize freetype" << std::endl;
        throw std::bad_exception();
    }
    if(FT_New_Face(lib, path, 0, &face)){
        std::cerr << "Failed to load font" << std::endl;
        throw std::bad_exception();
    }
    FT_Set_Pixel_Sizes(face, 0, font_size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(unsigned int c = 0; c < 256; c++){
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cerr << "Failed to load character: " << static_cast<char>(c) << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character{texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), static_cast<unsigned int>(face->glyph->advance.x)};
        characters.insert({c, character});
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(lib);
}

void Font::render(std::string_view text, float x, float y, float scale, glm::vec3 color) const {
    program.SetUniformMatrixf<4, 4>("projection", glm::value_ptr(window->ortho_project()));
    program.SetUniformf("textColor", color.r, color.g, color.b);
    for(std::string_view::iterator c = text.begin(); c != text.end(); c++){
        Character ch = characters.at(*c);
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        float vertices[24] = {
            xpos,   ypos+h, 0.0f, 0.0f,
            xpos,   ypos,   0.0f, 1.0f,
            xpos+w, ypos,   1.0f, 1.0f,
            xpos,   ypos+h, 0.0f, 0.0f,
            xpos+w, ypos,   1.0f, 1.0f,
            xpos+w, ypos+h, 1.0f, 0.0f
        };
        glBindTexture(GL_TEXTURE_2D, ch.id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 24*sizeof(float), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        if(*c == ' ') x += (ch.Advance >> 6) * scale;
        else x += (ch.Bearing.x + ch.Size.x) * scale;
    }
}
