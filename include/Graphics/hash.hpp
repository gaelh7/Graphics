#pragma once

#include <glm/gtx/hash.hpp>

class Point;
struct Physical;

template<>
struct std::hash<Point>{
    size_t operator()(const Point& p) const;
};

bool operator==(const Point& p1, const Point& p2);

bool operator!=(const Point& p1, const Point& p2);

template<>
struct std::hash<Physical>{
    size_t operator()(const Physical& p) const;
};

bool operator==(const Physical& p1, const Physical& p2);
