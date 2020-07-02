#include <Graphics/input.hpp>

std::map<int, std::pair<std::function<void()>, std::function<void()>>> InputHandler::key_bindings = {{GLFW_KEY_UNKNOWN, {[](){}, [](){}}}};

void InputHandler::bind_key(int key, std::function<void()> press_action, std::function<void()> release_action){
    key_bindings.insert({key, {press_action, release_action}});
}

void InputHandler::unbind_key(int key){
    key_bindings.erase(key);
}

void InputHandler::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    std::map<int, std::pair<std::function<void ()>, std::function<void ()>>>::iterator it = key_bindings.find(key);
    if(it != key_bindings.end())
        if(action == GLFW_PRESS){
            it->second.first();
        }
        else if(action == GLFW_RELEASE){
            it->second.second();
        }
}
