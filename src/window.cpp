#include "Graphics/window.hpp"
#include <stb_image.h>
#include <exception>

using namespace gmh;

Window::Window(unsigned int width, unsigned int height, const char* title): win(nullptr), frame_last(std::chrono::high_resolution_clock::now()), time(0), frames(0), w(width), h(height) {
    if(!glfwInit()) throw std::bad_exception();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(w, h, title, nullptr, nullptr);
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
        self->w = width;
        self->h = height;
        glViewport(0, 0, width, height);
    });
}

void Window::resize(unsigned int width, unsigned int height){
    glViewport(0, 0, width, height);
    glfwSetWindowSize(win, width, height);
    w = w;
    h  = h;
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
