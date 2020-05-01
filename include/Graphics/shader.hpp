#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <Graphics/render.hpp>

struct ShaderSource{
    std::string VertexShader, FragmentShader;
};

class Shader{
    private:
        ShaderSource src;
        const char* path;
        static ShaderSource ParseShader(const char* filepath);
        static unsigned int CompileShader(unsigned int type, const std::string &source);
        static unsigned int CreateShaders(const ShaderSource prg);
    public:
        unsigned int id;
        Shader(const char* filepath);
        ~Shader();
        inline void bind() const {GLCALL(glUseProgram(id));};
        template<typename... data> void SetUniformf(const char* uniform, data... v0) const {
            float data[] = {v0...};
            switch (sizeof...(v0)){
                case 1:
                    GLCALL(glUniform1f(glGetUniformLocation(id, uniform), data[0]));
                    break;
                case 2:
                    GLCALL(glUniform2f(glGetUniformLocation(id, uniform), data[0], data[1]));
                    break;
                case 3:
                    GLCALL(glUniform3f(glGetUniformLocation(id, uniform), data[0], data[1], data[2]));
                    break;
                case 4:
                    GLCALL(glUniform4f(glGetUniformLocation(id, uniform), data[0], data[1], data[2], data[3]));
                    break;
            }
        };
};
