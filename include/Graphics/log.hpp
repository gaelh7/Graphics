#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef NDEBUG
    #define EXIT(x) _Exit(x)
#else
    #define EXIT(x)
#endif

static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam){
    std::cerr << message << std::endl;
    if(severity == GL_DEBUG_SEVERITY_HIGH){
        EXIT(type);
    }
}

static void GLFWErrorCallback(int error_code, const char* message){
    std::cerr << "GLFW Error " << std::hex << error_code << ": " << message << std::endl;
    EXIT(error_code);
}

static inline void enableDebug(){
    glfwSetErrorCallback(GLFWErrorCallback);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLErrorCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
