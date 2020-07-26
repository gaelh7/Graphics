#pragma once

#include <glm/gtx/hash.hpp>

namespace gmh {
    class Point;
    struct Physical;

    bool operator==(const Point& p1, const Point& p2);

    bool operator!=(const Point& p1, const Point& p2);

    bool operator==(const Physical& p1, const Physical& p2);
}

template<>
struct std::hash<gmh::Point>{
    size_t operator()(const gmh::Point& p) const;
};

template<>
struct std::hash<gmh::Physical>{
    size_t operator()(const gmh::Physical& p) const;
};
