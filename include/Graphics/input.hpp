#pragma once

#include <unordered_map>
#include <functional>
#include <GLFW/glfw3.h>

class InputHandler {
    static std::unordered_map<int, std::pair<std::function<void()>, std::function<void()>>> key_bindings;

    public:
        static void bind_key(int key, std::function<void()> press_action, std::function<void()> release_action);

        static void unbind_key(int key);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void mouse_callback(GLFWwindow* window, double dx, double dy);

        static void scroll_callback(GLFWwindow* window, double dx, double dy);
};
