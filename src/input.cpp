#include "Graphics/input.hpp"

using namespace gmh;

void InputHandler::key_callback(int key, int action, int mods){
    std::unordered_map<int, std::pair<std::function<void(int)>, std::function<void(int)>>>::iterator it = key_bindings.find(key);
    if(it != key_bindings.end())
        if(action == GLFW_PRESS){
            it->second.first(mods);
        }
        else if(action == GLFW_RELEASE){
            it->second.second(mods);
        }
}

InputHandler::InputHandler(): mouse_callback([](double, double){}), scroll_callback([](double, double){}) {}

void InputHandler::bind(const Window& window){
    static InputHandler* ptr = this;
    glfwSetCursorPosCallback(window.handle(), [](GLFWwindow* win, double x, double y){ptr->mouse_callback(x, y);});
    glfwSetScrollCallback(window.handle(), [](GLFWwindow* win, double x, double y){ptr->scroll_callback(x, y);});
    glfwSetKeyCallback(window.handle(), [](GLFWwindow* win, int key, int scancode, int action, int mods){ptr->key_callback(key, action, mods);});
    glfwSetMouseButtonCallback(window.handle(), [](GLFWwindow* win, int button, int action, int mods){ptr->key_callback(button, action, mods);});
}

void InputHandler::bind_key(int key, std::function<void(int)> press_action, std::function<void(int)> release_action){
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

void InputHandler::set_mode(Window* window, int mode, int value) const {
    glfwSetInputMode(window->handle(), mode, value);
}

TextInput::TextInput(): buffer("") {
    for(std::pair<char, char> key: shift_map){
        if(key.first >= 'a' && key.first <= 'z'){
            bind_key(key.second, [this, key](int mods){
                if(!(mods & GLFW_MOD_CAPS_LOCK) ^ !(mods & GLFW_MOD_SHIFT)) buffer.push_back(key.second);
                else buffer.push_back(key.first);
            }, [](int mods){});
        }
        else{
            bind_key(key.first, [this, key](int mods){
                if(mods & GLFW_MOD_SHIFT) buffer.push_back(key.second);
                else buffer.push_back(key.first);
            }, [](int mods){});
        }
    }
    bind_key(GLFW_KEY_SPACE, [this](int mods){buffer.push_back(' ');}, [](int mods){});
    bind_key(GLFW_KEY_BACKSPACE, [this](int mods){
        if(buffer.size() > 0) buffer.pop_back();
    }, [](int mods){});
}

void TextInput::bind(const Window& window){
    glfwSetInputMode(window.handle(), GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    glfwSetCursorPosCallback(window.handle(), [](GLFWwindow* win, double x, double y){get().mouse_callback(x, y);});
    glfwSetScrollCallback(window.handle(), [](GLFWwindow* win, double x, double y){get().scroll_callback(x, y);});
    glfwSetKeyCallback(window.handle(), [](GLFWwindow* win, int key, int scancode, int action, int mods){get().key_callback(key, action, mods);});
    glfwSetMouseButtonCallback(window.handle(), [](GLFWwindow* win, int button, int action, int mods){get().key_callback(button, action, mods);});
}


const std::unordered_map<char, char> TextInput::shift_map = {
    {'a', 'A'},
    {'b', 'B'},
    {'c', 'C'},
    {'d', 'D'},
    {'e', 'E'},
    {'f', 'F'},
    {'g', 'G'},
    {'h', 'H'},
    {'i', 'I'},
    {'j', 'J'},
    {'k', 'K'},
    {'l', 'L'},
    {'m', 'M'},
    {'n', 'N'},
    {'o', 'O'},
    {'p', 'P'},
    {'q', 'Q'},
    {'r', 'R'},
    {'s', 'S'},
    {'t', 'T'},
    {'u', 'U'},
    {'v', 'V'},
    {'w', 'W'},
    {'x', 'X'},
    {'y', 'Y'},
    {'z', 'Z'},
    {'1', '!'},
    {'2', '@'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '^'},
    {'7', '&'},
    {'8', '*'},
    {'9', '('},
    {'0', ')'},
    {'-', '_'},
    {'=', '+'},
    {'`', '~'},
    {'[', '{'},
    {']', '}'},
    {';', ':'},
    {',', '<'},
    {'.', '>'},
    {'/', '?'},
    {'\\', '|'},
    {'\'', '"'}
};
