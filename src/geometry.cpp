#include <algorithm>
#include <glm/gtx/norm.hpp>
#include "Graphics/geometry.hpp"
#include "Graphics/gmath.hpp"

using namespace gmh;

Point::Point(): pos({0, 0, 0}), vel(0, 0, 0), model(1.0) {}

Point::Point(glm::vec3 pos): pos(pos), vel(0, 0, 0), model(1.0) {}

float Point::dist(const Point &obj) const {
    return glm::distance(pos, obj.pos);
}

float Point::dist(const Line &obj) const {
    return glm::length(glm::cross(obj.vertices[0]->pos - pos, obj.vertices[0]->pos - obj.vertices[1]->pos) / obj.vertices[0]->dist(*obj.vertices[1]));
}

float Point::dist(const LinSeg &obj) const {
    return glm::dot(pos - obj.vertices[0]->pos, obj.vertices[1]->pos - obj.vertices[0]->pos) > 0 && glm::dot(pos - obj.vertices[1]->pos, obj.vertices[0]->pos - obj.vertices[1]->pos) > 0 ? glm::length(glm::cross(obj.vertices[0]->pos - pos, obj.vertices[0]->pos - obj.vertices[1]->pos) / obj.vertices[0]->dist(*obj.vertices[1])):std::min(dist(*obj.vertices[0]), dist(*obj.vertices[1]));
}

float Point::dist(const Plane &obj) const {
    return std::abs(glm::dot(obj.normVec(), pos - obj.vertices[0]->pos));
}

float Point::dist(const Polygon &obj) const {
    for(std::shared_ptr<LinSeg> edge: obj.edges)
        if(glm::dot(glm::cross(edge->v[1]->pos - edge->v[0]->pos, pos - edge->v[0]->pos), obj.normVec()) < 0){
            std::vector<float> distances(obj.edges.size());
            std::transform(obj.edges.begin(), obj.edges.end(), distances.begin(), [this](std::shared_ptr<LinSeg> lin){
                return dist(*lin);
            });
            return *std::min_element(distances.begin(), distances.end());
        }
    return std::abs(glm::dot(obj.normVec(), pos - obj.vertices[0]->pos));
}

float Point::dist(const Polyhedron &obj) const {
    bool contained = std::all_of(obj.faces.begin(), obj.faces.end(), [this](std::shared_ptr<Polygon> face){
        return face->sign_dist(*this) >= 0;
    });
    if(contained) return 0;
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return dist(*face);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Point::intersect(const Point &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(this->pos):nullptr;
}

std::unique_ptr<Point> Point::intersect(const Line &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(this->pos):nullptr;
}

std::unique_ptr<Point> Point::intersect(const LinSeg &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(this->pos):nullptr;
}

std::unique_ptr<Point> Point::intersect(const Plane &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(this->pos):nullptr;
}

std::unique_ptr<Point> Point::intersect(const Polygon &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(this->pos):nullptr;
}

std::unique_ptr<Point> Point::intersect(const Polyhedron &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(this->pos):nullptr;
}

glm::vec3 Point::direction(const Point &obj) const {
    if(equals(obj))
        return {0, 0, 0};
    return glm::normalize(obj.pos - pos);
}

bool Point::contains(const Point &obj) const {
    if(typeid(obj) == typeid(Point)) return dist(obj) < 1e-5;
    else if (obj.isSpace() > isSpace() || obj.dim() > dim()) return false;
    return std::none_of(obj.vertices.begin(), obj.vertices.end(), [this](std::shared_ptr<Point> vert){
        return dist(*vert) >= 1e-5;
    });
}

bool Point::equals(const Point &obj) const {
    return obj.contains(*this) && contains(obj);
}

void Point::update(float dt) {
    model = glm::translate(model, dt*vel);
    pos += dt*vel;
    for(std::shared_ptr<Point> p: v)
        p->pos += dt*vel;
}

void Point::transform(glm::mat4 mat) {
    model = mat*model;
    pos = mat*glm::vec4(pos, 1.0);
    for(std::shared_ptr<Point> p: v)
        p->pos = mat*glm::vec4(p->pos, 1.0);
}

Point& Point::operator=(const Point& p){
    model = p.model;
    pos = p.pos;
    vel = p.vel;
    v = p.v;
    return *this;
}

Line::Line(){
    v = {std::make_shared<Point>(glm::vec3(0, 0, 0)), std::make_shared<Point>(glm::vec3(1, 0, 0))};
}

Line::Line(Point p1, Point p2){
    if(p1.equals(p2))
        throw std::invalid_argument("Inputs must have different positions");
    v = {std::make_shared<Point>(p1.pos), std::make_shared<Point>(p2.pos)};
}

Line::Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
    if(p1->equals(*p2))
        throw std::invalid_argument("Inputs must have different positions");
    v = {p1, p2};
}

Line::Line(std::vector<Point> vert): Line(vert[0], vert[1]){}

Line::Line(std::vector<std::shared_ptr<Point>> vert): Line(vert[0], vert[1]){}

float Line::dist(const Point &obj) const {
    return glm::length(glm::cross(v[0]->pos - obj.pos, v[0]->pos - v[1]->pos) / v[0]->dist(*v[1]));
}

float Line::dist(const Line &obj) const {
    glm::vec3 vec = glm::cross(dirVec(), obj.dirVec());
    return glm::length2(vec) < 1e-5 ? dist(*obj.vertices[0]):std::abs(glm::dot(vec, v[0]->pos - obj.vertices[1]->pos))/glm::length(vec);
}

float Line::dist(const LinSeg &obj) const {
    glm::vec3 c = glm::cross(dirVec(), obj.dirVec());
    if(glm::length2(c) < 1e-5) return dist(*obj.vertices[0]);
    float t = glm::determinant(glm::mat3(obj.vertices[0]->pos - v[0]->pos, dirVec(), c))/glm::length2(c);
    return t < 0 || t > glm::distance(obj.vertices[1]->pos, obj.vertices[0]->pos) ? std::min(dist(*obj.vertices[0]), dist(*obj.vertices[1])):dist(static_cast<Line>(obj));
}

float Line::dist(const Plane &obj) const {
    return std::abs(glm::dot(obj.normVec(), dirVec())) < 1e-5 ? obj.dist(*v[0]):0;
}

float Line::dist(const Polygon &obj) const {
    std::unique_ptr<Point> p = intersect(static_cast<Plane>(obj));
    if(p && obj.contains(*p)) return 0;
    std::vector<float> distances(obj.edges.size());
    std::transform(obj.edges.begin(), obj.edges.end(), distances.begin(), [this](std::shared_ptr<LinSeg> lin){
        return dist(*lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Line::dist(const Polyhedron &obj) const {
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return dist(*face);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Line::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> Line::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<Line>(*obj.vertices[0], *obj.vertices[1]);
    else if(contains(*obj.vertices[0])) return std::make_unique<Point>(obj.vertices[0]->pos);
    else if(obj.contains(*v[0])) return std::make_unique<Point>(v[0]->pos);
    glm::vec3 vec1 = glm::cross(obj.dirVec(), obj.vertices[0]->pos - v[0]->pos);
    glm::vec3 vec2 = glm::cross(obj.dirVec(), dirVec());
    return std::make_unique<Point>(v[0]->pos + (sign(glm::dot(vec1, vec2)))*(glm::length(vec1)/glm::length(vec2))*dirVec());
}

std::unique_ptr<Point> Line::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return contains(obj) ? std::make_unique<LinSeg>(*obj.vertices[0], *obj.vertices[1]):intersect(static_cast<Line>(obj));
}

std::unique_ptr<Point> Line::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(obj.contains(*this)) return std::make_unique<Line>(*v[0], *v[1]);
    if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(v[0]->pos - obj.normVec()*obj.sign_dist(*v[0]));
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> Line::intersect(const Polygon &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(v[0]->pos - obj.normVec()*obj.sign_dist(*v[0]));
    if(std::abs(glm::dot(dirVec(), obj.normVec())) < 1e-5){
        std::vector<Point> points;
        points.reserve(obj.edges.size());
        for(std::shared_ptr<LinSeg> edge: obj.edges){
            if(std::unique_ptr<Point> inter = intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> Line::intersect(const Polyhedron &obj) const {
    std::vector<Point> points;
    points.reserve(obj.faces.size());
    for(std::shared_ptr<Polygon> face: obj.faces){
        if(std::unique_ptr<Point> inter = intersect(*face); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    if(points.size() == 0) return nullptr;
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

glm::vec3 Line::dirVec() const {
    return v[0]->direction(*v[1]);
}

Point Line::project(const Point &obj) const {
    return Point(v[0]->pos + dirVec()*glm::dot(obj.pos - v[0]->pos, dirVec()));
}

float Line::angle(const Line &lobj, glm::vec3* axisptr){
    glm::vec3 axis = axisptr ? *axisptr:glm::cross(dirVec(), lobj.dirVec());
    axis = glm::normalize(axis);
    float theta = std::atan2(glm::determinant(glm::mat3(dirVec(), lobj.dirVec(), axis)), glm::dot(dirVec(), lobj.dirVec()));
    return theta >= 0 ? theta:theta + 2*glm::pi<float>();
}

float LinSeg::dist(const Point &obj) const {
    return glm::dot(obj.pos - v[0]->pos, v[1]->pos - v[0]->pos) > 0 && glm::dot(obj.pos - v[1]->pos, v[0]->pos - v[1]->pos) > 0 ? glm::length(glm::cross(v[0]->pos - obj.pos, v[0]->pos - v[1]->pos) / v[0]->dist(*v[1])):std::min(obj.dist(*v[0]), obj.dist(*v[1]));
}

float LinSeg::dist(const Line &obj) const {
    glm::vec3 c = glm::cross(obj.dirVec(), dirVec());
    if(glm::length2(c) < 1e-5) return obj.dist(*v[0]);
    float t = glm::determinant(glm::mat3(v[0]->pos - obj.vertices[0]->pos, obj.dirVec(), c))/glm::length2(c);
    return t < 0 || t > glm::distance(v[1]->pos, v[0]->pos) ? std::min(obj.dist(*v[0]), obj.dist(*v[1])):obj.dist(static_cast<Line>(*this));
}

float LinSeg::dist(const LinSeg &obj) const {
    if(contains(*obj.vertices[0]) || obj.contains(*v[0])) return 0;
    glm::vec3 c = glm::cross(dirVec(), obj.dirVec());
    glm::vec3 t = obj.vertices[0]->pos - v[0]->pos;
    float c_squared = glm::length2(c);
    float t0 = glm::determinant(glm::mat3(t, obj.dirVec(), c))/c_squared;
    float t1 = glm::determinant(glm::mat3(t, dirVec(), c))/c_squared;
    if(c_squared < 1e-5 || t0 < 0 || t0 > length()) return std::min(obj.dist(*v[0]), obj.dist(*v[1]));
    else if(t1 < 0 || t1 > obj.length()) return std::min(dist(*obj.vertices[0]), dist(*obj.vertices[1]));
    return std::abs(glm::dot(c, v[0]->pos - obj.vertices[1]->pos))/glm::length(c);
}

float LinSeg::dist(const Plane &obj) const {
    if((sign(obj.sign_dist(*v[0])) ^ sign(obj.sign_dist(*v[1]))) < 0) return 0;
    return std::min(obj.dist(*v[0]), obj.dist(*v[1]));
}

float LinSeg::dist(const Polygon &obj) const {
    std::unique_ptr<Point> p = intersect(static_cast<Plane>(obj));
    if(p && obj.contains(*p)) return 0;
    std::vector<float> distances(obj.edges.size());
    std::transform(obj.edges.begin(), obj.edges.end(), distances.begin(), [this](std::shared_ptr<LinSeg> lin){
        return dist(*lin);
    });
    float min = *std::min_element(distances.begin(), distances.end());
    min = std::min(min, obj.dist(*v[0]));
    min = std::min(min, obj.dist(*v[1]));
    return min;
}

float LinSeg::dist(const Polyhedron &obj) const {
    for(std::shared_ptr<Point> p: v) if(obj.contains(*p)) return 0;
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return dist(*face);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> LinSeg::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> LinSeg::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return obj.contains(*this) ? std::make_unique<LinSeg>(*v[0], *v[1]):obj.intersect(static_cast<Line>(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<LinSeg>(*obj.vertices[0], *obj.vertices[1]);
    else if(obj.contains(*this)) return std::make_unique<LinSeg>(*v[0], *v[1]);
    if(glm::length2(glm::cross(dirVec(), obj.dirVec())) < 1e-5){
        if(obj.contains(*v[0])){
            if(v[0]->equals(*obj.vertices[0]) || v[0]->equals(*obj.vertices[1])) return std::make_unique<Point>(v[0]->pos);
            return contains(*obj.vertices[0]) ? std::make_unique<LinSeg>(*v[0], *obj.vertices[0]):std::make_unique<LinSeg>(*v[0], *obj.vertices[1]);
        }
        if(obj.contains(*v[1])){
            if(v[1]->equals(*obj.vertices[0]) || v[1]->equals(*obj.vertices[1])) return std::make_unique<Point>(v[1]->pos);
            return contains(*obj.vertices[0]) ? std::make_unique<LinSeg>(*v[1], *obj.vertices[0]):std::make_unique<LinSeg>(*v[1], *obj.vertices[1]);
        }
    }
    return Line::intersect(obj);
}

std::unique_ptr<Point> LinSeg::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(obj.contains(*this)) return std::make_unique<LinSeg>(*v[0], *v[1]);
    if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(v[0]->pos - obj.normVec()*obj.sign_dist(*v[0]));
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const Polygon &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(v[0]->pos - obj.normVec()*obj.sign_dist(*v[0]));
    else if(obj.contains(*this)) return std::make_unique<LinSeg>(*v[0], *v[1]);
    if(std::abs(glm::dot(dirVec(), obj.normVec())) < 1e-5){
        std::vector<Point> points;
        points.reserve(obj.edges.size() + 1);
        if(obj.contains(*v[0])) points.push_back(*v[0]);
        for(std::shared_ptr<LinSeg> edge: obj.edges){
            if(std::unique_ptr<Point> inter = intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const Polyhedron &obj) const {
    if(obj.contains(*this)) return std::make_unique<LinSeg>(*v[0], *v[1]);
    std::vector<Point> points;
    if(obj.contains(*v[0])) points.push_back(*v[0]);
    for(std::shared_ptr<Polygon> face: obj.faces){
        if(std::unique_ptr<Point> inter = intersect(*face); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    if(points.size() == 0) return nullptr;
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

float LinSeg::length() const {
    return glm::distance(v[0]->pos, v[1]->pos);
}

Plane::Plane(){
    v = {std::make_shared<Point>(glm::vec3(0, 0, 0)), std::make_shared<Point>(glm::vec3(1, 0, 0)), std::make_shared<Point>(glm::vec3(0, 1, 0))};
}

Plane::Plane(Point p1, Point p2, Point p3){
    if(Line(p1, p2).contains(p3)) throw std::invalid_argument("Inputs cannot be collinear");
    v = {std::make_shared<Point>(p1.pos), std::make_shared<Point>(p2.pos), std::make_shared<Point>(p3.pos)};
}

Plane::Plane(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<Point> p3){
    if(Line(*p1, *p2).contains(*p3)) throw std::invalid_argument("Inputs cannot be collinear");
    v = {p1, p2, p3};
}

Plane::Plane(std::vector<Point> vert): Plane(vert[0], vert[1], vert[2]){}

Plane::Plane(std::vector<std::shared_ptr<Point>> vert): Plane(vert[0], vert[1], vert[2]){}

glm::vec3 Plane::normVec() const {
    glm::vec3 vec = glm::cross(v[1]->pos - v[0]->pos, v[2]->pos - v[0]->pos);
    return glm::normalize(vec);
}

Point Plane::project(const Point &obj) const {
    return Point(obj.pos - normVec()*sign_dist(obj));
}

float Plane::sign_dist(const Point &obj) const {
    return glm::dot(normVec(), obj.pos - v[0]->pos);
}

float Plane::dist(const Point &obj) const {
    return std::abs(glm::dot(normVec(), obj.pos - v[0]->pos));
}

float Plane::dist(const Line &obj) const {
    return std::abs(glm::dot(normVec(), obj.dirVec())) < 1e-5 ? dist(*obj.vertices[0]):0;
}

float Plane::dist(const LinSeg &obj) const {
    if((sign(sign_dist(*obj.vertices[0])) ^ sign(sign_dist(*obj.vertices[1]))) < 0) return 0;
    return std::min(dist(*obj.vertices[0]), dist(*obj.vertices[1]));
}

float Plane::dist(const Plane &obj) const {
    return glm::length2(glm::cross(normVec(), obj.normVec())) < 1e-5 ? dist(*obj.vertices[0]):0;
}

float Plane::dist(const Polygon &obj) const {
    std::vector<float> distances(obj.edges.size());
    std::transform(obj.edges.begin(), obj.edges.end(), distances.begin(), [this](std::shared_ptr<LinSeg> lin){
        return dist(*lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Plane::dist(const Polyhedron &obj) const {
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return dist(*face);
    });
    return *std::min_element(distances.begin(), distances.end());
};

std::unique_ptr<Point> Plane::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> Plane::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<Line>(*obj.vertices[0], *obj.vertices[1]);
    if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0]->pos - normVec()*sign_dist(*obj.vertices[0]));
    return obj.intersect(project(obj));
}

std::unique_ptr<Point> Plane::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<LinSeg>(*obj.vertices[0], *obj.vertices[1]);
    if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0]->pos - normVec()*sign_dist(*obj.vertices[0]));
    return obj.intersect(project(obj));
}

std::unique_ptr<Point> Plane::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(contains(obj)) return std::make_unique<Plane>(*obj.vertices[0], *obj.vertices[1], *obj.vertices[2]);
    std::unique_ptr<Point> x = obj.intersect(Line(v[0], v[1]));
    if(!x) x = obj.intersect(Line(v[0], v[2]));
    return std::make_unique<Line>(*x, Point(x->pos + glm::cross(normVec(), obj.normVec())));
}

std::unique_ptr<Point> Plane::intersect(const Polygon &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)){
        std::vector<Point> vert;
        vert.reserve(obj.vertices.size());
        std::transform(obj.vertices.begin(), obj.vertices.end(), std::back_inserter(vert), [](std::shared_ptr<Point> p){
            return Point(p->pos);
        });
        return std::make_unique<Polygon>(vert);
    }
    std::vector<Point> points;
    points.reserve(obj.edges.size());
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        if(std::unique_ptr<Point> inter = intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Plane::intersect(const Polyhedron &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        if(std::unique_ptr<Point> inter = intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point> out;
    out.reserve(points.size());
    std::copy_if(points.begin(), points.end(), std::back_inserter(out), [&out](Point p1){
        return std::none_of(out.begin(), out.end(), [&p1](Point p2){return p1.equals(p2);});
    });
    if(out.size() == 1) return std::make_unique<Point>(out[0]);
    else if(out.size() == 2) return std::make_unique<LinSeg>(out[0], out[1]);
    else return std::make_unique<Polygon>(out);
}

Polygon::Polygon(){
    pos = {1.0/3.0, 1.0/3.0, 1.0/3.0};
    e = {std::make_shared<LinSeg>(v[0], v[1]), std::make_shared<LinSeg>(v[1], v[2]), std::make_shared<LinSeg>(v[2], v[0])};
}

Polygon::Polygon(std::vector<Point> vert): Plane(vert[0], vert[1], vert[2]){
    Plane check(vert[0], vert[1], vert[2]);
    for(Point p: vert)
        if(!check.contains(p)) throw std::invalid_argument("Inputs must be coplanar");
    v.reserve(vert.size());
    std::transform(vert.begin() + 3, vert.end(), std::back_inserter(v), [](Point p){
        return std::make_shared<Point>(p.pos);
    });
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: v) pos += p->pos;
    pos /= v.size();
    Point center{pos};
    Line l{center, Point(v[0]->pos)};
    glm::vec3 vec = glm::cross(v[0]->direction(*v[1]), v[0]->direction(*v[2]));
    std::sort(v.begin(), v.end(), [&l, &vec, &center](std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
        return l.angle(Line(center, *p1), &vec) < l.angle(Line(center, *p2), &vec);
    });
    for(unsigned int i = 0; i < v.size(); i++){
        e.push_back(std::make_shared<LinSeg>(v[i], v[(i+1)%v.size()]));
        if(i > 0 && e[i]->dirVec() == e[i-1]->dirVec() || i == v.size() - 1 && e[i]->dirVec() == e[0]->dirVec())
            throw std::invalid_argument("Inputs cannot be collinear");
        if(i > 0 && e[i-1]->angle(*e[i%e.size()], &vec) > glm::pi<float>() || i == v.size() - 1 && e[i]->angle(*e[0], &vec) > glm::pi<float>())
            throw std::invalid_argument("Inputs must define a convex polygon");
    }
}

Polygon::Polygon(std::vector<std::shared_ptr<Point>> vert){
    Plane check(vert[0], vert[1], vert[2]);
    for(std::shared_ptr<Point> p: vert)
        if(!check.contains(*p)) throw std::invalid_argument("Inputs must be coplanar");
    v = vert;
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: v) pos += p->pos;
    pos /= v.size();
    Point center{pos};
    Line l{center, *v[0]};
    glm::vec3 vec = glm::cross(v[0]->direction(*v[1]), v[0]->direction(*v[2]));
    std::sort(v.begin(), v.end(), [&l, &vec, &center](std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
        return l.angle(Line(center, *p1), &vec) < l.angle(Line(center, *p2), &vec);
    });
    for(unsigned int i = 0; i < v.size(); i++){
        e.push_back(std::make_shared<LinSeg>(v[i], v[(i+1)%v.size()]));
        if(i > 0 && e[i]->dirVec() == e[i-1]->dirVec() || i == v.size() - 1 && e[i]->dirVec() == e[0]->dirVec())
            throw std::invalid_argument("Inputs cannot be collinear");
        if(i > 0 && e[i-1]->angle(*e[i%e.size()], &vec) > glm::pi<float>() || i == v.size() - 1 && e[i]->angle(*e[0], &vec) > glm::pi<float>())
            throw std::invalid_argument("Inputs must define a convex polygon");
    }
}

float Polygon::dist(const Point &obj) const {
    for(std::shared_ptr<LinSeg> edge: e)
        if(glm::dot(glm::cross(edge->vertices[1]->pos - edge->vertices[0]->pos, obj.pos - edge->vertices[0]->pos), normVec()) < 0){
            std::vector<float> distances(e.size());
            std::transform(e.begin(), e.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
                return obj.dist(*lin);
            });
            return *std::min_element(distances.begin(), distances.end());
        }
    return std::abs(glm::dot(normVec(), obj.pos - v[0]->pos));
}

float Polygon::dist(const Line &obj) const {
    Plane pl(v[0], v[1], v[2]);
    std::unique_ptr<Point> p = pl.intersect(obj);
    if(p && contains(*p)) return 0;
    std::vector<float> distances(e.size());
    std::transform(e.begin(), e.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
        return obj.dist(*lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polygon::dist(const LinSeg &obj) const {
    std::unique_ptr<Point> p = static_cast<Plane>(*this).intersect(obj);
    if(p && contains(*p)) return 0;
    std::vector<float> distances(e.size());
    std::transform(e.begin(), e.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
        return obj.dist(*lin);
    });
    float min = *std::min_element(distances.begin(), distances.end());
    min = std::min(min, dist(*obj.vertices[0]));
    min = std::min(min, dist(*obj.vertices[1]));
    return min;
}

float Polygon::dist(const Plane &obj) const {
    std::vector<float> distances(e.size());
    std::transform(e.begin(), e.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
        return obj.dist(*lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polygon::dist(const Polygon &obj) const {
    std::vector<float> dist1(e.size());
    std::vector<float> dist2(obj.e.size());
    std::transform(e.begin(), e.end(), dist1.begin(), [&obj](std::shared_ptr<LinSeg> lin){return obj.dist(*lin);});
    std::transform(obj.e.begin(), obj.e.end(), dist2.begin(), [this](std::shared_ptr<LinSeg> lin){return dist(*lin);});
    return std::min(*std::min_element(dist1.begin(), dist1.end()), *std::min_element(dist2.begin(), dist2.end()));
}

float Polygon::dist(const Polyhedron &obj) const {
    for(std::shared_ptr<Point> p: v) if(obj.contains(*p)) return 0;
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return dist(*face);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Polygon::intersect(const Point &obj) const{
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> Polygon::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0]->pos - normVec()*sign_dist(*obj.vertices[0]));
    if(std::abs(glm::dot(obj.dirVec(), normVec())) < 1e-5){
        std::vector<Point> points;
        for(std::shared_ptr<LinSeg> edge: e){
            if(std::unique_ptr<Point> inter = obj.intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return obj.intersect(project(obj));
}

std::unique_ptr<Point> Polygon::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0]->pos - normVec()*sign_dist(*obj.vertices[0]));
    else if(contains(obj)) return std::make_unique<LinSeg>(*obj.vertices[0], *obj.vertices[1]);
    if(std::abs(glm::dot(obj.dirVec(), normVec())) < 1e-5){
        std::vector<Point> points;
        if(contains(*obj.vertices[0])) points.push_back(*obj.vertices[0]);
        else if(contains(*obj.vertices[1])) points.push_back(*obj.vertices[1]);
        for(std::shared_ptr<LinSeg> edge: e){
            if(std::unique_ptr<Point> inter = obj.intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return obj.intersect(project(obj));
}

std::unique_ptr<Point> Polygon::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(obj.contains(*this)){
        std::vector<Point> vert;
        vert.reserve(vert.size());
        std::transform(vertices.begin(), vertices.end(), std::back_inserter(vert), [](std::shared_ptr<Point> p){
            return *p;
        });
        return std::make_unique<Polygon>(vert);
    }
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: e){
        if(std::unique_ptr<Point> inter = obj.intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Polygon::intersect(const Polygon &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Point> p: v)
        if(obj.contains(*p)) points.push_back(*p);
    for(std::shared_ptr<Point> p: obj.vertices)
        if(contains(*p)) points.push_back(*p);
    if(glm::length2(glm::cross(normVec(), obj.normVec())) < 1e-5){
        for(std::shared_ptr<LinSeg> edge1: e){
            for(std::shared_ptr<LinSeg> edge2: obj.e){
                if(std::unique_ptr<Point> inter = edge1->intersect(*edge2); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
            }
        }
    }
    else{
        for(std::shared_ptr<LinSeg> edge: e){
            if(std::unique_ptr<Point> inter = obj.intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        for(std::shared_ptr<LinSeg> edge: obj.e){
            if(std::unique_ptr<Point> inter = intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    std::vector<Point> out;
    std::copy_if(points.begin(), points.end(), std::back_inserter(out), [&out](Point p1){
        return std::none_of(out.begin(), out.end(), [&p1](Point p2){return p1.equals(p2);});
    });
    if(out.size() > 2) return std::make_unique<Polygon>(out);
    else if(out.size() == 2) return std::make_unique<LinSeg>(out[0], out[1]);
    else return std::make_unique<Point>(out[0].pos);
}

std::unique_ptr<Point> Polygon::intersect(const Polyhedron &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Point> p: v){
        if(obj.contains(*p)) points.push_back(*p);
    }
    for(std::shared_ptr<LinSeg> edge: e){
        for(std::shared_ptr<Polygon> face: obj.faces){
            if(std::unique_ptr<Point> inter = edge->intersect(*face); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        if(std::unique_ptr<Point> inter = intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point> out;
    std::copy_if(points.begin(), points.end(), std::back_inserter(out), [&out](Point p1){
        return std::none_of(out.begin(), out.end(), [&p1](Point p2){return p1.equals(p2);});
    });
    if(out.size() > 2) return std::make_unique<Polygon>(out);
    else if(out.size() == 2) return std::make_unique<LinSeg>(out[0], out[1]);
    else return std::make_unique<Point>(out[0].pos);
}

float Polygon::area() const {
    float area = 0;
    for(std::shared_ptr<LinSeg> edge: e) area += glm::length(glm::cross(edge->vertices[0]->pos - pos, edge->vertices[0]->pos - edge->vertices[1]->pos))/2;
    return area;
}

Polygon& Polygon::operator=(const Polygon& poly){
    model = poly.model;
    pos = poly.pos;
    vel = poly.vel;
    v = poly.v;
    e = poly.e;
    return *this;
}

Polyhedron::Polyhedron(){
    pos = {0.25, 0.25, 0.25};
    v = {std::make_shared<Point>(glm::vec3(0, 0, 0)), std::make_shared<Point>(glm::vec3(1, 0, 0)), std::make_shared<Point>(glm::vec3(0, 1, 0)), std::make_shared<Point>(glm::vec3(0, 0, 1))};
    e = {std::make_shared<LinSeg>(v[0], v[1]), std::make_shared<LinSeg>(v[0], v[2]), std::make_shared<LinSeg>(v[0], v[3]), std::make_shared<LinSeg>(v[1], v[2]), std::make_shared<LinSeg>(v[1], v[3]), std::make_shared<LinSeg>(v[2], v[3])};
    f = {std::make_shared<Polygon>(v[0], v[3], v[1]), std::make_shared<Polygon>(v[0], v[2], v[3]), std::make_shared<Polygon>(v[0], v[1], v[2]), std::make_shared<Polygon>(v[3], v[2], v[1])};
}

Polyhedron::Polyhedron(std::vector<Point> vert){
    v.reserve(vert.size());
    std::transform(vert.begin(), vert.end(), std::back_inserter(v), [](Point p){
        return std::make_shared<Point>(p.pos);
    });
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: v) pos += p->pos;
    pos /= v.size();
    Point center{pos};
    std::vector<std::vector<std::shared_ptr<Point>>> points;
    for(unsigned int i = 3; i <= v.size(); i++){
        std::vector<std::vector<std::shared_ptr<Point>>> comb = combinations<std::shared_ptr<Point>>(v.begin(), v.end(), i);
        points.insert(points.end(), comb.begin(), comb.end());
    }
    for(std::vector<std::vector<std::shared_ptr<Point>>>::reverse_iterator it = points.rbegin(); it != points.rend(); it++){
        try{
            std::shared_ptr<Polygon> face = std::make_shared<Polygon>(*it);
            if(face->sign_dist(center) < 0){
                std::swap((*it)[1], (*it)[2]);
                face = std::make_shared<Polygon>(*it);
            }
            bool side = std::all_of(v.begin(), v.end(), [&face](std::shared_ptr<Point> p){
                return face->sign_dist(*p) >= -1e-5;
            });
            bool subface = std::any_of(f.begin(), f.end(), [&face](std::shared_ptr<Polygon> face2){
                return !face->contains(*face2) && face2->contains(*face);
            });
            if(side && !subface) f.push_back(face);
        }
        catch(std::invalid_argument){}
    }
    if(f.size() == 0) throw std::invalid_argument("Inputs cannot be coplanar");
    std::vector<std::shared_ptr<LinSeg>> lines;
    for(std::shared_ptr<Polygon> face: f) lines.insert(lines.end(), face->edges.begin(), face->edges.end());
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(e), [this](std::shared_ptr<LinSeg> l1){
        return std::find_if(e.begin(), e.end(), [&l1](std::shared_ptr<LinSeg> l2){return l1->equals(*l2);}) == e.end();
    });
    if(f.size() + v.size() - e.size() != 2)
        throw std::invalid_argument("Inputs must define a convex polyhedron");
}

Polyhedron::Polyhedron(std::vector<std::shared_ptr<Point>> vert){
    v = vert;
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: v) pos += p->pos;
    pos /= v.size();
    Point center{pos};
    std::vector<std::vector<std::shared_ptr<Point>>> points;
    for(unsigned int i = 3; i < v.size(); i++){
        std::vector<std::vector<std::shared_ptr<Point>>> comb = combinations<std::shared_ptr<Point>>(v.begin(), v.end(), i);
        points.insert(points.end(), comb.begin(), comb.end());
    }
    for(std::vector<std::vector<std::shared_ptr<Point>>>::reverse_iterator it = points.rbegin(); it != points.rend(); it++){
        try{
            std::shared_ptr<Polygon> face = std::make_shared<Polygon>(*it);
            if(face->sign_dist(center) < 0){
                std::swap((*it)[1], (*it)[2]);
                face = std::make_shared<Polygon>(*it);
            }
            bool side = std::all_of(v.begin(), v.end(), [&face](std::shared_ptr<Point> p){
                return face->sign_dist(*p) >= -1e-5;
            });
            bool subface = std::any_of(f.begin(), f.end(), [&face](std::shared_ptr<Polygon> face2){
                return !face->contains(*face2) && face2->contains(*face);
            });
            if(side && !subface) f.push_back(face);
        }
        catch(std::invalid_argument){}
    }
    if(f.size() == 0) throw std::invalid_argument("Inputs cannot be coplanar");
    std::vector<std::shared_ptr<LinSeg>> lines;
    for(std::shared_ptr<Polygon> face: f) lines.insert(lines.end(), face->edges.begin(), face->edges.end());
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(e), [this](std::shared_ptr<LinSeg> l1){
        return std::find_if(e.begin(), e.end(), [&l1](std::shared_ptr<LinSeg> l2){return l1->equals(*l2);}) == e.end();
    });
    if(f.size() + v.size() - e.size() != 2) throw std::invalid_argument("Inputs must define a convex polyhedron");
}

float Polyhedron::dist(const Point &obj) const {
    bool contained = std::all_of(f.begin(), f.end(), [&obj](std::shared_ptr<Polygon> face){
        return face->sign_dist(obj) >= 0;
    });
    if(contained) return 0;
    std::vector<float> distances(f.size());
    std::transform(f.begin(), f.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const Line &obj) const {
    std::vector<float> distances(f.size());
    std::transform(f.begin(), f.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const LinSeg &obj) const {
    for(std::shared_ptr<Point> p: obj.vertices) if(contains(*p)) return 0;
    std::vector<float> distances(f.size());
    std::transform(f.begin(), f.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const Plane &obj) const {
    std::vector<float> distances(f.size());
    std::transform(f.begin(), f.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}


float Polyhedron::dist(const Polygon &obj) const {
    for(std::shared_ptr<Point> p: obj.vertices) if(contains(*p)) return 0;
    std::vector<float> distances(f.size());
    std::transform(f.begin(), f.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}


float Polyhedron::dist(const Polyhedron &obj) const {
    for(std::shared_ptr<Point> p: obj.vertices) if(contains(*p)) return 0;
    std::vector<float> distances(f.size());
    std::transform(f.begin(), f.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Polyhedron::intersect(const Point &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(obj.pos):nullptr;
}

std::unique_ptr<Point> Polyhedron::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Polygon> face: f){
        if(std::unique_ptr<Point> inter = face->intersect(obj); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Polyhedron::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Point> p: obj.vertices)
        if(contains(*p)) points.push_back(*p);
    for(std::shared_ptr<Polygon> face: f){
        if(std::unique_ptr<Point> inter = face->intersect(obj); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    return std::distance(points.begin(), it) == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Polyhedron::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    points.reserve(e.size());
    for(std::shared_ptr<LinSeg> edge: e){
        if(std::unique_ptr<Point> inter = obj.intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point> out;
    out.reserve(points.size());
    std::copy_if(points.begin(), points.end(), std::back_inserter(out), [&out](Point p1){
        return std::none_of(out.begin(), out.end(), [&p1](Point p2){return p1.equals(p2);});
    });
    if(out.size() > 2) return std::make_unique<Polygon>(out);
    else if(out.size() == 2) return std::make_unique<LinSeg>(out[0], out[1]);
    else return std::make_unique<Point>(out[0].pos);
}

std::unique_ptr<Point> Polyhedron::intersect(const Polygon &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Point> p: obj.vertices){
        if(contains(*p)) points.push_back(*p);
    }
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        for(std::shared_ptr<Polygon> face: f){
            if(std::unique_ptr<Point> inter = edge->intersect(*face); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<LinSeg> edge: e){
        if(std::unique_ptr<Point> inter = obj.intersect(*edge); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point> out;
    std::copy_if(points.begin(), points.end(), std::back_inserter(out), [&out](Point p1){
        return std::none_of(out.begin(), out.end(), [&p1](Point p2){return p1.equals(p2);});
    });
    if(out.size() > 2) return std::make_unique<Polygon>(out);
    else if(out.size() == 2) return std::make_unique<LinSeg>(out[0], out[1]);
    else return std::make_unique<Point>(out[0].pos);
}

std::unique_ptr<Point> Polyhedron::intersect(const Polyhedron &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: e){
        for(std::shared_ptr<Polygon> face: obj.f){
            if(std::unique_ptr<Point> inter = edge->intersect(*face); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<LinSeg> edge: obj.e){
        for(std::shared_ptr<Polygon> face: f){
            if(std::unique_ptr<Point> inter = edge->intersect(*face); inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<Point> p: v){
        if(obj.contains(*p)) points.push_back(*p);
    }
    for(std::shared_ptr<Point> p: obj.vertices){
        if(contains(*p)) points.push_back(*p);
    }
    std::vector<Point> out;
    std::copy_if(points.begin(), points.end(), std::back_inserter(out), [&out](Point p1){
        return std::none_of(out.begin(), out.end(), [&p1](Point p2){return p1.equals(p2);});
    });
    if(out.size() > 2){
        try{
            return std::make_unique<Polygon>(out);
        }
        catch(std::invalid_argument){
            return std::make_unique<Polyhedron>(out);
        }
    }
    else if(out.size() == 2) return std::make_unique<LinSeg>(out[0], out[1]);
    else return std::make_unique<Point>(out[0].pos);
}

float Polyhedron::volume() const {
    float vol = 0;
    for(std::shared_ptr<Polygon> face: f) vol += glm::dot(face->normVec(), pos - face->pos)*face->area()/3;
    return vol;
}

Polyhedron& Polyhedron::operator=(const Polyhedron& poly){
    model = poly.model;
    pos = poly.pos;
    vel = poly.vel;
    v = poly.v;
    e = poly.e;
    f = poly.f;
    return *this;
}
