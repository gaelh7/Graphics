#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>

namespace gmh{
    struct ShaderSource {
        std::string VertexShader, FragmentShader;
    };

    class Shader {
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
            inline void bind() const {glUseProgram(id);}
            template<typename... floats>
            void SetUniformf(const char* uniform, float v0, floats... v1) const {
                float data[] = {v0, v1...};
                switch (sizeof...(v1)){
                    case 0:
                        glUniform1fv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                    case 1:
                        glUniform2fv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                    case 2:
                        glUniform3fv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                    case 3:
                        glUniform4fv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                }
            };
            template<typename... ints>
            void SetUniformi(const char* uniform, int v0, ints... v1) const {
                int data[] = {v0, v1...};
                switch (sizeof...(v1)){
                    case 0:
                        glUniform1iv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                    case 1:
                        glUniform2iv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                    case 2:
                        glUniform3iv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                    case 3:
                        glUniform4iv(glGetUniformLocation(id, uniform), 1, data);
                        break;
                }
            };
            template<int col, int row>
            void SetUniformMatrixf(const char* uniform, const float* data, bool transpose = false){
                switch (row){
                    case 2:
                        switch (col){
                            case 2:
                                glUniformMatrix2fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 3:
                                glUniformMatrix2x3fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 4:
                                glUniformMatrix2x4fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                        }
                        break;
                    case 3:
                        switch (col){
                            case 2:
                                glUniformMatrix3x4fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 3:
                                glUniformMatrix3fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 4:
                                glUniformMatrix3x4fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                        }
                        break;
                    case 4:
                        switch (col){
                            case 2:
                                glUniformMatrix4x2fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 3:
                                glUniformMatrix4x3fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 4:
                                glUniformMatrix4fv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                        }
                        break;
                }
            }
            template<int col, int row>
            void SetUniformMatrixd(const char* uniform, const double* data, bool transpose = false){
                switch (col){
                    case 2:
                        switch (row){
                            case 2:
                                glUniformMatrix2dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 3:
                                glUniformMatrix2x3dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 4:
                                glUniformMatrix2x4dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                        }
                        break;
                    case 3:
                        switch (row){
                            case 2:
                                glUniformMatrix3x2dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 3:
                                glUniformMatrix3dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 4:
                                glUniformMatrix3x4dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                        }
                        break;
                    case 4:
                        switch (row){
                            case 2:
                                glUniformMatrix4x2dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 3:
                                glUniformMatrix4x3dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                            case 4:
                                glUniformMatrix4dv(glGetUniformLocation(id, uniform), 1, transpose, data);
                                break;
                        }
                        break;
                }
            }
    };
}
