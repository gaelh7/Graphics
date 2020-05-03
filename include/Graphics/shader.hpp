#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <Graphics/log.hpp>

struct ShaderSource{
    std::string VertexShader, FragmentShader;
};

class Shader{
    private:
        const ShaderSource src;
        std::string path;
        const unsigned int id;
        static ShaderSource ParseShader(const char* filepath);
        static unsigned int CompileShader(unsigned int type, const std::string &source);
        static unsigned int CreateShaders(const ShaderSource prg);
    public:
        Shader(const char* filepath);
        ~Shader();
        inline void bind() const {GLCALL(glUseProgram(id));};
        template<typename... data> void SetUniformf(const char* uniform, float v0, data... v1) const {
            float data[] = {v0, v1...};
            switch (sizeof...(v1)){
                case 0:
                    GLCALL(glUniform1fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 1:
                    GLCALL(glUniform2fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 2:
                    GLCALL(glUniform3fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 3:
                    GLCALL(glUniform4fv(glGetUniformLocation(id, uniform), 1, data));
                    break;
            }
        };
        template<typename... data> void SetUniformi(const char* uniform, int v0, data... v1) const {
            int data[] = {v0, v1...};
            switch (sizeof...(v1)){
                case 0:
                    GLCALL(glUniform1iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 1:
                    GLCALL(glUniform2iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 2:
                    GLCALL(glUniform3iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
                case 3:
                    GLCALL(glUniform4iv(glGetUniformLocation(id, uniform), 1, data));
                    break;
            }
        };
};
