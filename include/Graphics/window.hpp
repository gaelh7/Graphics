#pragma once

#include <GLFW/glfw3.h>

namespace gmh{
    class Window {
        GLFWwindow* win;
        public:
            int width, height;
            Window(int width, int height, const char* title);
            void resize(int w, int h);
            inline GLFWwindow* handle() const {return win;};
    };
}
