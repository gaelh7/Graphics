#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static size_t allocations = 0;
static size_t deletions = 0;
static size_t bytes_allocated = 0;

#ifndef NDEBUG
    #define EXIT std::terminate()
#else
    #define EXIT
#endif
void * operator new(size_t size){
    allocations++;
    bytes_allocated += size;
    return std::malloc(size);
}

void operator delete(void * p){
    deletions++;
    std::free(p);
}


static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam){
    std::cerr << message << std::endl;
    if(severity == GL_DEBUG_SEVERITY_HIGH){
        EXIT;
    }
}

static void GLFWErrorCallback(int error_code, const char* message){
    std::cerr << "GLFW Error " << std::hex << error_code << ": " << message << std::endl;
    EXIT;
}

static inline void enableDebug(){
    glfwSetErrorCallback(GLFWErrorCallback);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLErrorCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
