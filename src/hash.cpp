#include "Graphics/hash.hpp"
#include "Graphics/geometry.hpp"
#include "Graphics/collision.hpp"

size_t std::hash<Point>::operator()(const Point& p) const {
    size_t seed = std::hash<glm::vec3>()(p.pos) ^ std::hash<int>()(p.dim()) ^ std::hash<bool>()(p.isSpace());
    for(std::shared_ptr<Point> ptr: p.vertices){
        seed ^= std::hash<glm::vec3>()(ptr->pos);
    }
    return seed;
}

bool operator==(const Point& p1, const Point& p2){
    std::hash<Point> h;
    return h(p1) == h(p2);
}

bool operator!=(const Point& p1, const Point& p2){
    std::hash<Point> h;
    return h(p1) != h(p2);
}

size_t std::hash<Physical>::operator()(const Physical& p) const {
    return std::hash<Point>()(*p.obj);
}

bool operator==(const Physical& p1, const Physical& p2){
    std::hash<Physical> h;
    return h(p1) == h(p2);
}
