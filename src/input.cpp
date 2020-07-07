#include "Graphics/input.hpp"

GLFWwindow* InputHandler::window = nullptr;
std::unordered_map<int, std::pair<std::function<void()>, std::function<void()>>> InputHandler::key_bindings = {{GLFW_KEY_UNKNOWN, {[](){}, [](){}}}};
std::function<void(double, double)> InputHandler::mouse_callback = [](double, double){};
std::function<void(double, double)> InputHandler::scroll_callback = [](double, double){};

void InputHandler::key_callback(int key, int action){
    std::unordered_map<int, std::pair<std::function<void ()>, std::function<void ()>>>::iterator it = key_bindings.find(key);
    if(it != key_bindings.end())
        if(action == GLFW_PRESS){
            it->second.first();
        }
        else if(action == GLFW_RELEASE){
            it->second.second();
        }
}

void InputHandler::init(GLFWwindow* win){
    window = win;
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y){mouse_callback(x, y);});
    glfwSetScrollCallback(window, [](GLFWwindow* win, double x, double y){scroll_callback(x, y);});
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods){key_callback(key, action);});
    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods){key_callback(button, action);});
}

void InputHandler::bind_key(int key, std::function<void()> press_action, std::function<void()> release_action){
    key_bindings.insert({key, {press_action, release_action}});
}

void InputHandler::unbind_key(int key){
    key_bindings.erase(key);
}

void InputHandler::set_cursor_pos(std::function<void(double x, double y)> action){
    mouse_callback = action;
}

void InputHandler::set_scroll(std::function<void(double x, double y)> action){
    scroll_callback = action;
}

void InputHandler::set_mode(int mode, int value){
    glfwSetInputMode(window, mode, value);
}
