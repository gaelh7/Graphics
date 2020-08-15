#pragma once

// #include <unordered_map>
#include <map>
#include <functional>
#include <string>
#include <sstream>
#include "Graphics/window.hpp"

namespace gmh{
    class InputHandler {
        public:
            std::unordered_map<int, std::pair<std::function<void(int)>, std::function<void(int)>>> key_bindings;
            std::function<void(double, double)> mouse_callback;
            std::function<void(double, double)> scroll_callback;
            void key_callback(int key, int action, int mods);
        public:
            InputHandler();
            void bind(const Window& window);
            void bind_key(int key, std::function<void(int)> press_action, std::function<void(int)> release_action);
            void unbind_key(int key);
            void set_cursor_pos(std::function<void(double, double)> action);
            void set_scroll(std::function<void(double, double)> action);
            void set_mode(Window* window, int mode, int value) const;

    };

    class TextInput: private InputHandler {
        std::string buffer;
        static const std::map<unsigned char, unsigned char> shift_map;
        TextInput();
        public:
            static TextInput& get(){
                static TextInput self;
                return self;
            }
            void bind(const Window& window);
            inline std::string text() const {return buffer;}
            TextInput(TextInput const&) = delete;
            void operator=(TextInput const&)  = delete;
    };
}
