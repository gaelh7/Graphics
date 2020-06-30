#pragma once

#include <iostream>
#include <glad/glad.h>

#ifndef NDEBUG
    #define ASSERT(x) if (!(x)) __debugbreak()
    #define GLCALL(x) GLClearError();\
                      x;\
                      ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define ASSERT(x) x
    #define GLCALL(x) x
#endif

static void GLClearError(){
    while(glGetError());
}

static bool GLLogCall(const char* function, const char* file, int line){
    if(GLenum error = glGetError()){
        std::cerr << "OpenGLError " << std::hex << error << ": " << file << "(" << std::dec << line << "): in " << function << std::endl;
        return false;
    }
    return true;
}
