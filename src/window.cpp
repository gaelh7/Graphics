#include "Graphics/window.hpp"
#include <stb_image.h>
#include <exception>

using namespace gmh;

Window::Window(unsigned int w, unsigned int h, const char* title): win(nullptr), frame_last(std::chrono::high_resolution_clock::now()), time(0), frames(0), width(w), height(h) {
    if(!glfwInit()) throw std::bad_exception();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!win){
        glfwTerminate();
        throw std::bad_exception();
    }
    glfwMakeContextCurrent(win);
    glfwSetWindowUserPointer(win, this);
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

void Window::resize(unsigned int w, unsigned int h){
    glViewport(0, 0, w, h);
    glfwSetWindowSize(win, w, h);
    width = w;
    height  = h;
}

void Window::setIcon(const char* path){
    GLFWimage icon;
    icon.pixels = stbi_load(path, &icon.width, &icon.height, 0, 4);
    glfwSetWindowIcon(win, 1, &icon);
    stbi_image_free(icon.pixels);
}

float Window::update(){
    float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - frame_last).count()/1000000000.0f;
    time += dt;
    frames++;
    glfwSwapBuffers(win);
    glfwPollEvents();
    frame_last = std::chrono::high_resolution_clock::now();
    return dt;
}
