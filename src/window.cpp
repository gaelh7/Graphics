#include "Graphics/window.hpp"
#include <glad/glad.h>
#include <exception>

using namespace gmh;

Window::Window(int w, int h, const char* title): width(w), height(h) {
    if(!glfwInit()) throw std::bad_exception();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!win){
        glfwTerminate();
        throw std::bad_exception();
    }
    glfwMakeContextCurrent(win);
    glfwSetWindowUserPointer(win, reinterpret_cast<void*>(this));
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        throw std::bad_exception();
    }
    glfwSetFramebufferSizeCallback(win, [](GLFWwindow* window, int width, int height){
        gmh::Window* self = reinterpret_cast<gmh::Window*>(glfwGetWindowUserPointer(window));
        self->width = width;
        self->height = height;
        glViewport(0, 0, width, height);
    });
}

void Window::resize(int w, int h){
    glViewport(0, 0, w, h);
    glfwSetWindowSize(win, w, h);
    width = w;
    height  = h;
}
