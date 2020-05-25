#pragma once

#include "Graphics/render.hpp"

struct Tangible {
    Visual* obj;
    bool fixed;
};

class CHandler {
    std::vector<std::vector<Tangible>> get_check();
    std::vector<Tangible> _tangible;
    public:
        CHandler(){};
        void operator()();
        void handle(){this->operator()();};
        void add(Visual* v, bool fixed = false);
        void add(Tangible t);
        void collision(Surface* s1, Surface* s2);
        void collision(Surface* s1, Solid* s2);
        void collision(Solid* s1, Surface* s2);
        void collision(Solid* s1, Solid* s2);
};
