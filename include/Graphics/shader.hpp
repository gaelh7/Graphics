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
        const ShaderSource src;
        const char* path;
        static ShaderSource ParseShader(const char* filepath);
        static unsigned int CompileShader(unsigned int type, const std::string &source);
        static unsigned int CreateShaders(const ShaderSource prg);
    public:
        const unsigned int id;
        Shader(const char* filepath);
        ~Shader();
        inline void bind() const {GLCALL(glUseProgram(id));};
        template<typename... data> void SetUniformf(const char* uniform, data... v0) const {
            float data[] = {v0...};
            switch (sizeof...(v0)){
                case 1:
                    GLCALL(glUniform1fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 2:
                    GLCALL(glUniform2fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 3:
                    GLCALL(glUniform3fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 4:
                    GLCALL(glUniform4fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
            }
        };
        template<typename... data> void SetUniformi(const char* uniform, data... v0) const {
            int data[] = {v0...};
            switch (sizeof...(v0)){
                case 1:
                    GLCALL(glUniform1iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 2:
                    GLCALL(glUniform2iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 3:
                    GLCALL(glUniform3iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 4:
                    GLCALL(glUniform4iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
            }
        };
};
