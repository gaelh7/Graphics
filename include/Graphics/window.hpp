#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

namespace gmh{
    class Window {
        GLFWwindow* win;
        std::chrono::steady_clock::time_point frame_last;
        float time;
        unsigned int frames;
        unsigned int w, h;
        public:
            unsigned int &width = w, &height = h;
            Window(unsigned int width, unsigned int height, const char* title);
            void resize(unsigned int w, unsigned int h);
            void setIcon(const char* path);
            float update();
            inline bool isOpen(){return !glfwWindowShouldClose(win);}
            inline void close(){glfwSetWindowShouldClose(win, true);}
            inline unsigned int current_frame() const {return frames;}
            inline float FPS() const {return frames/time;}
            inline float aspect() const {return static_cast<float>(w)/static_cast<float>(h);}
            inline glm::mat4 ortho_project() const {return glm::ortho(0.0f, static_cast<float>(w), 0.0f, static_cast<float>(h));};
            inline void clear(unsigned int bitfield) const {glClear(bitfield);}
            inline GLFWwindow* handle() const {return win;}
    };
}
