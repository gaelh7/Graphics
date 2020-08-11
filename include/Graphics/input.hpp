#pragma once

#include <unordered_map>
#include <functional>
#include "Graphics/window.hpp"

namespace gmh{
    class InputHandler {
        static Window* window;
        static std::unordered_map<int, std::pair<std::function<void()>, std::function<void()>>> key_bindings;
        static std::function<void(double, double)> mouse_callback;
        static std::function<void(double, double)> scroll_callback;
        static void key_callback(int key, int action);

        public:
            static void init(Window* win);

            static void bind_key(int key, std::function<void()> press_action, std::function<void()> release_action);

            static void unbind_key(int key);

            static void set_cursor_pos(std::function<void(double, double)> action);

            static void set_scroll(std::function<void(double, double)> action);

            static void set_mode(int mode, int value);

    };
}
