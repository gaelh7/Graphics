#include <algorithm>
#include <glm/gtx/norm.hpp>
#include "Graphics/geometry.hpp"
#include "Graphics/gmath.hpp"

Point::Point(): pos({0, 0, 0}), vel(0, 0, 0) {}

Point::Point(glm::vec3 pos): pos(pos), vel(0, 0, 0) {}

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
        if(glm::dot(glm::cross(edge->vertices[1]->pos - edge->vertices[0]->pos, pos - edge->vertices[0]->pos), obj.normVec()) < 0){
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
        return face->dist(*this);
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
    if(obj.dim() == 0) return dist(obj) < 1e-5;
    else if (obj.isSpace() > isSpace() || obj.dim() > dim()) return false;
    return std::none_of(obj.vertices.begin(), obj.vertices.end(), [this](std::shared_ptr<Point> vert){
        return dist(*vert) >= 1e-5;
    });
}

bool Point::equals(const Point &obj) const {
    return obj.contains(*this) && contains(obj);
}

Line::Line(Point p1, Point p2){
    if(p1.equals(p2))
        throw std::invalid_argument("Inputs must have different positions");
    vertices = {std::make_shared<Point>(p1.pos), std::make_shared<Point>(p2.pos)};
}

Line::Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
    if(p1->equals(*p2))
        throw std::invalid_argument("Inputs must have different positions");
    vertices = {p1, p2};
}

Line::Line(std::vector<Point> vert): Line(vert[0], vert[1]){}

Line::Line(std::vector<std::shared_ptr<Point>> vert): Line(vert[0], vert[1]){}

float Line::dist(const Point &obj) const {
    return glm::length(glm::cross(vertices[0]->pos - obj.pos, vertices[0]->pos - vertices[1]->pos) / vertices[0]->dist(*vertices[1]));
}

float Line::dist(const Line &obj) const {
    glm::vec3 vec = glm::cross(dirVec(), obj.dirVec());
    return glm::length2(vec) < 1e-5 ? dist(*obj.vertices[0]):std::abs(glm::dot(vec, vertices[0]->pos - obj.vertices[1]->pos))/glm::length(vec);
}

float Line::dist(const LinSeg &obj) const {
    glm::vec3 c = glm::cross(dirVec(), obj.dirVec());
    if(glm::length2(c) < 1e-5) return dist(*obj.vertices[0]);
    float t = glm::determinant(glm::mat3(obj.vertices[0]->pos - vertices[0]->pos, dirVec(), c))/glm::length2(c);
    return t < 0 || t > glm::distance(obj.vertices[1]->pos, obj.vertices[0]->pos) ? std::min(dist(*obj.vertices[0]), dist(*obj.vertices[1])):dist(static_cast<Line>(obj));
}

float Line::dist(const Plane &obj) const {
    return std::abs(glm::dot(obj.normVec(), dirVec())) < 1e-5 ? obj.dist(*vertices[0]):0;
}

float Line::dist(const Polygon &obj) const {
    const Plane pl(obj.vertices[0], obj.vertices[1], obj.vertices[2]);
    std::unique_ptr<Point> p = pl.intersect(*this);
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
        return face->dist(*this);
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
    else if(obj.contains(*vertices[0])) return std::make_unique<Point>(vertices[0]->pos);
    glm::vec3 vec1 = glm::cross(obj.dirVec(), obj.vertices[0]->pos - vertices[0]->pos);
    glm::vec3 vec2 = glm::cross(obj.dirVec(), dirVec());
    return std::make_unique<Point>(vertices[0]->pos + (sign(glm::dot(vec1, vec2)))*(glm::length(vec1)/glm::length(vec2))*dirVec());
}

std::unique_ptr<Point> Line::intersect(const LinSeg &obj) const {
    return contains(obj) ? std::make_unique<LinSeg>(*obj.vertices[0], *obj.vertices[1]):intersect(static_cast<Line>(obj));
}

std::unique_ptr<Point> Line::intersect(const Plane &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    if(obj.contains(*this)) return std::make_unique<Line>(*vertices[0], *vertices[1]);
    if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0]->pos - obj.normVec()*obj.sign_dist(*vertices[0]));
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> Line::intersect(const Polygon &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0]->pos - obj.normVec()*obj.sign_dist(*vertices[0]));
    if(std::abs(glm::dot(dirVec(), obj.normVec())) < 1e-5){
        std::vector<Point> points;
        for(std::shared_ptr<LinSeg> edge: obj.edges){
            std::unique_ptr<Point> inter = intersect(*edge);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), it));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> Line::intersect(const Polyhedron &obj) const {
    std::vector<Point> points;
    for(std::shared_ptr<Polygon> face: obj.faces){
        std::unique_ptr<Point> inter = intersect(*face);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    if(points.size() == 0) return nullptr;
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

glm::vec3 Line::dirVec() const {
    return vertices[0]->direction(*vertices[1]);
}

Point Line::project(const Point &obj) const {
    return Point(vertices[0]->pos + dirVec()*glm::dot(obj.pos - vertices[0]->pos, dirVec()));
}

float Line::angle(const Line &lobj, glm::vec3* axisptr){
    glm::vec3 axis = axisptr ? *axisptr:glm::cross(dirVec(), lobj.dirVec());
    axis = glm::normalize(axis);
    float theta = std::atan2(glm::determinant(glm::mat3(dirVec(), lobj.dirVec(), axis)), glm::dot(dirVec(), lobj.dirVec()));
    return theta >= 0 ? theta:theta + 2*glm::pi<float>();
}

LinSeg::LinSeg(Point p1, Point p2): Line(p1,p2){}

LinSeg::LinSeg(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2): Line(p1,p2){}

LinSeg::LinSeg(std::vector<Point> vert): Line(vert[0], vert[1]){}

LinSeg::LinSeg(std::vector<std::shared_ptr<Point>> vert): Line(vert[0], vert[1]){}

float LinSeg::dist(const Point &obj) const {
    return glm::dot(obj.pos - vertices[0]->pos, vertices[1]->pos - vertices[0]->pos) > 0 && glm::dot(obj.pos - vertices[1]->pos, vertices[0]->pos - vertices[1]->pos) > 0 ? glm::length(glm::cross(vertices[0]->pos - obj.pos, vertices[0]->pos - vertices[1]->pos) / vertices[0]->dist(*vertices[1])):std::min(obj.dist(*vertices[0]), obj.dist(*vertices[1]));
}

float LinSeg::dist(const Line &obj) const {
    glm::vec3 c = glm::cross(obj.dirVec(), dirVec());
    if(glm::length2(c) < 1e-5) return obj.dist(*vertices[0]);
    float t = glm::determinant(glm::mat3(vertices[0]->pos - obj.vertices[0]->pos, obj.dirVec(), c))/glm::length2(c);
    return t < 0 || t > glm::distance(vertices[1]->pos, vertices[0]->pos) ? std::min(obj.dist(*vertices[0]), obj.dist(*vertices[1])):obj.dist(static_cast<Line>(*this));
}

float LinSeg::dist(const LinSeg &obj) const {
    glm::vec3 c = glm::cross(dirVec(), obj.dirVec());
    glm::vec3 t = obj.vertices[0]->pos - vertices[0]->pos;
    float c_squared = glm::length2(c);
    float t0 = glm::determinant(glm::mat3(t, obj.dirVec(), c))/c_squared;
    float t1 = glm::determinant(glm::mat3(t, dirVec(), c))/c_squared;
    if(c_squared < 1e-5 || t0 < 0 || t0 > length()) return std::min(obj.dist(*vertices[0]), obj.dist(*vertices[1]));
    else if(t1 < 0 || t1 > obj.length()) return std::min(dist(*obj.vertices[0]), dist(*obj.vertices[1]));
    return std::abs(glm::dot(c, vertices[0]->pos - obj.vertices[1]->pos))/glm::length(c);
}

float LinSeg::dist(const Plane &obj) const {
    if((sign(obj.sign_dist(*vertices[0])) ^ sign(obj.sign_dist(*vertices[1]))) < 0) return 0;
    return std::min(obj.dist(*vertices[0]), obj.dist(*vertices[1]));
}

float LinSeg::dist(const Polygon &obj) const {
    std::unique_ptr<Point> p = intersect(static_cast<Plane>(obj));
    if(p && obj.contains(*p)) return 0;
    std::vector<float> distances(obj.edges.size());
    std::transform(obj.edges.begin(), obj.edges.end(), distances.begin(), [this](std::shared_ptr<LinSeg> lin){
        return dist(*lin);
    });
    float min = *std::min_element(distances.begin(), distances.end());
    min = std::min(min, obj.dist(*vertices[0]));
    min = std::min(min, obj.dist(*vertices[1]));
    return min;
}

float LinSeg::dist(const Polyhedron &obj) const {
    for(std::shared_ptr<Point> p: vertices) if(obj.contains(*p)) return 0;
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return face->dist(*this);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> LinSeg::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> LinSeg::intersect(const Line &obj) const {
    return obj.contains(*this) ? std::make_unique<LinSeg>(*vertices[0], *vertices[1]):obj.intersect(*this);
}

std::unique_ptr<Point> LinSeg::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<LinSeg>(*obj.vertices[0], *obj.vertices[1]);
    else if(obj.contains(*this)) return std::make_unique<LinSeg>(*vertices[0], *vertices[1]);
    if(glm::length2(glm::cross(dirVec(), obj.dirVec())) < 1e-5){
        if(obj.contains(*vertices[0])){
            if(vertices[0]->equals(*obj.vertices[0]) || vertices[0]->equals(*obj.vertices[1])) return std::make_unique<Point>(vertices[0]->pos);
            return contains(*obj.vertices[0]) ? std::make_unique<LinSeg>(*vertices[0], *obj.vertices[0]):std::make_unique<LinSeg>(*vertices[0], *obj.vertices[1]);
        }
        if(obj.contains(*vertices[1])){
            if(vertices[1]->equals(*obj.vertices[0]) || vertices[1]->equals(*obj.vertices[1])) return std::make_unique<Point>(vertices[1]->pos);
            return contains(*obj.vertices[0]) ? std::make_unique<LinSeg>(*vertices[1], *obj.vertices[0]):std::make_unique<LinSeg>(*vertices[1], *obj.vertices[1]);
        }
    }
    return Line::intersect(obj);
}

std::unique_ptr<Point> LinSeg::intersect(const Plane &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    if(obj.contains(*this)) return std::make_unique<LinSeg>(*vertices[0], *vertices[1]);
    if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0]->pos - obj.normVec()*obj.sign_dist(*vertices[0]));
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const Polygon &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0]->pos - obj.normVec()*obj.sign_dist(*vertices[0]));
    else if(obj.contains(*this)) return std::make_unique<LinSeg>(*vertices[0], *vertices[1]);
    if(std::abs(glm::dot(dirVec(), obj.normVec())) < 1e-5){
        std::vector<Point> points;
        if(obj.contains(*vertices[0])) points.push_back(*vertices[0]);
        else if(contains(*vertices[1])) points.push_back(*vertices[1]);
        for(std::shared_ptr<LinSeg> edge: obj.edges){
            std::unique_ptr<Point> inter = intersect(*edge);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), it));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return intersect(obj.project(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const Polyhedron &obj) const {
    if(obj.contains(*this)) return std::make_unique<LinSeg>(*vertices[0], *vertices[1]);
    std::vector<Point> points;
    if(obj.contains(*vertices[0])) points.push_back(*vertices[0]);
    else if(contains(*vertices[1])) points.push_back(*vertices[1]);
    for(std::shared_ptr<Polygon> face: obj.faces){
        std::unique_ptr<Point> inter = intersect(*face);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    if(points.size() == 0) return nullptr;
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

float LinSeg::length() const {
    return glm::distance(vertices[0]->pos, vertices[1]->pos);
}

Plane::Plane(Point p1, Point p2, Point p3){
    if(Line(p1, p2).contains(p3)) throw std::invalid_argument("Inputs cannot be collinear");
    vertices = {std::make_shared<Point>(p1.pos), std::make_shared<Point>(p2.pos), std::make_shared<Point>(p3.pos)};
}

Plane::Plane(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<Point> p3){
    if(Line(*p1, *p2).contains(*p3)) throw std::invalid_argument("Inputs cannot be collinear");
    vertices = {p1, p2, p3};
}

Plane::Plane(std::vector<Point> vert): Plane(vert[0], vert[1], vert[2]){}

Plane::Plane(std::vector<std::shared_ptr<Point>> vert): Plane(vert[0], vert[1], vert[2]){}

glm::vec3 Plane::normVec() const {
    glm::vec3 vec = glm::cross(vertices[1]->pos - vertices[0]->pos, vertices[2]->pos - vertices[0]->pos);
    return glm::normalize(vec);
}

Point Plane::project(const Point &obj) const {
    return Point(obj.pos - normVec()*sign_dist(obj));
}

float Plane::sign_dist(const Point &obj) const {
    return glm::dot(normVec(), obj.pos - vertices[0]->pos);
}

float Plane::dist(const Point &obj) const {
    return std::abs(glm::dot(normVec(), obj.pos - vertices[0]->pos));
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
        return face->dist(*this);
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
    std::unique_ptr<Point> x = obj.intersect(Line(vertices[0], vertices[1]));
    if(!x) x = obj.intersect(Line(vertices[0], vertices[2]));
    return std::make_unique<Line>(*x, Point(x->pos + glm::cross(normVec(), obj.normVec())));
}

std::unique_ptr<Point> Plane::intersect(const Polygon &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    std::vector<Point> v(obj.vertices.size());
    std::transform(obj.vertices.begin(), obj.vertices.end(), v.begin(), [](std::shared_ptr<Point> p){
        return Point(p->pos);
    });
    if(contains(obj)) return std::make_unique<Polygon>(v);
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        std::unique_ptr<Point> inter = intersect(*edge);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Plane::intersect(const Polyhedron &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        std::unique_ptr<Point> inter = intersect(*edge);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    if(points.size() == 1) return std::make_unique<Point>(points[0]);
    else if(points.size() == 2) return std::make_unique<LinSeg>(points[0], points[1]);
    else return std::make_unique<Polygon>(points);
}

Polygon::Polygon(std::vector<Point> vert): Plane(vert[0], vert[1], vert[2]){
    Plane check(vert[0], vert[1], vert[2]);
    for(Point p: vert)
        if(!check.contains(p)) throw std::invalid_argument("Inputs must be coplanar");
    vertices.resize(vert.size());
    std::transform(vert.begin(), vert.end(), vertices.begin(), [](Point p){
        return std::make_shared<Point>(p.pos);
    });
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: vertices) pos += p->pos;
    pos /= vertices.size();
    Point center{pos};
    Line l{center, Point(vertices[0]->pos)};
    glm::vec3 vec = glm::cross(vertices[0]->direction(*vertices[1]), vertices[0]->direction(*vertices[2]));
    std::sort(vertices.begin(), vertices.end(), [&l, &vec, &center](std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
        return l.angle(Line(center, *p1), &vec) < l.angle(Line(center, *p2), &vec);
    });
    for(unsigned int i = 0; i < vertices.size(); i++){
        edges.push_back(std::make_shared<LinSeg>(vertices[i], vertices[(i+1)%vertices.size()]));
        if(i > 0 && edges[i]->dirVec() == edges[i-1]->dirVec() || i == vertices.size() - 1 && edges[i]->dirVec() == edges[0]->dirVec())
            throw std::invalid_argument("Inputs cannot be collinear");
        if(i > 0 && edges[i-1]->angle(*edges[i%edges.size()], &vec) > glm::pi<float>() || i == vertices.size() - 1 && edges[i]->angle(*edges[0], &vec) > glm::pi<float>())
            throw std::invalid_argument("Inputs must define a convex polygon");
    }
}

Polygon::Polygon(std::vector<std::shared_ptr<Point>> vert){
    Plane check(vert[0], vert[1], vert[2]);
    for(std::shared_ptr<Point> p: vert)
        if(!check.contains(*p)) throw std::invalid_argument("Inputs must be coplanar");
    vertices = vert;
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: vertices) pos += p->pos;
    pos /= vertices.size();
    Point center{pos};
    Line l{center, *vertices[0]};
    glm::vec3 vec = glm::cross(vertices[0]->direction(*vertices[1]), vertices[0]->direction(*vertices[2]));
    std::sort(vertices.begin(), vertices.end(), [&l, &vec, &center](std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
        return l.angle(Line(center, *p1), &vec) < l.angle(Line(center, *p2), &vec);
    });
    for(unsigned int i = 0; i < vertices.size(); i++){
        edges.push_back(std::make_shared<LinSeg>(vertices[i], vertices[(i+1)%vertices.size()]));
        if(i > 0 && edges[i]->dirVec() == edges[i-1]->dirVec() || i == vertices.size() - 1 && edges[i]->dirVec() == edges[0]->dirVec())
            throw std::invalid_argument("Inputs cannot be collinear");
        if(i > 0 && edges[i-1]->angle(*edges[i%edges.size()], &vec) > glm::pi<float>() || i == vertices.size() - 1 && edges[i]->angle(*edges[0], &vec) > glm::pi<float>())
            throw std::invalid_argument("Inputs must define a convex polygon");
    }
}

float Polygon::dist(const Point &obj) const {
    for(std::shared_ptr<LinSeg> edge: edges)
        if(glm::dot(glm::cross(edge->vertices[1]->pos - edge->vertices[0]->pos, obj.pos - edge->vertices[0]->pos), normVec()) < 0){
            std::vector<float> distances(edges.size());
            std::transform(edges.begin(), edges.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
                return obj.dist(*lin);
            });
            return *std::min_element(distances.begin(), distances.end());
        }
    return std::abs(glm::dot(normVec(), obj.pos - vertices[0]->pos));
}

float Polygon::dist(const Line &obj) const {
    Plane pl(vertices[0], vertices[1], vertices[2]);
    std::unique_ptr<Point> p = pl.intersect(obj);
    if(p && contains(*p)) return 0;
    std::vector<float> distances(edges.size());
    std::transform(edges.begin(), edges.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
        return obj.dist(*lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polygon::dist(const LinSeg &obj) const {
    std::unique_ptr<Point> p = static_cast<Plane>(*this).intersect(obj);
    if(p && contains(*p)) return 0;
    std::vector<float> distances(edges.size());
    std::transform(edges.begin(), edges.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
        return obj.dist(*lin);
    });
    float min = *std::min_element(distances.begin(), distances.end());
    min = std::min(min, dist(*obj.vertices[0]));
    min = std::min(min, dist(*obj.vertices[1]));
    return min;
}

float Polygon::dist(const Plane &obj) const {
    std::vector<float> distances(edges.size());
    std::transform(edges.begin(), edges.end(), distances.begin(), [&obj](std::shared_ptr<LinSeg> lin){
        return obj.dist(*lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polygon::dist(const Polygon &obj) const {
    std::vector<float> dist1(edges.size());
    std::vector<float> dist2(obj.edges.size());
    std::transform(edges.begin(), edges.end(), dist1.begin(), [&obj](std::shared_ptr<LinSeg> lin){return obj.dist(*lin);});
    std::transform(obj.edges.begin(), obj.edges.end(), dist2.begin(), [this](std::shared_ptr<LinSeg> lin){return dist(*lin);});
    return std::min(*std::min_element(dist1.begin(), dist1.end()), *std::min_element(dist2.begin(), dist2.end()));
}

float Polygon::dist(const Polyhedron &obj) const {
    for(std::shared_ptr<Point> p: vertices) if(obj.contains(*p)) return 0;
    std::vector<float> distances(obj.faces.size());
    std::transform(obj.faces.begin(), obj.faces.end(), distances.begin(), [this](std::shared_ptr<Polygon> face){
        return face->dist(*this);
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
        for(std::shared_ptr<LinSeg> edge: edges){
            std::unique_ptr<Point> inter = obj.intersect(*edge);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), it));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
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
        for(std::shared_ptr<LinSeg> edge: edges){
            std::unique_ptr<Point> inter = obj.intersect(*edge);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), it));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
    }
    return obj.intersect(project(obj));
}

std::unique_ptr<Point> Polygon::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(obj.contains(*this)){
        std::vector<Point> v(vertices.size());
        std::transform(vertices.begin(), vertices.end(), v.begin(), [](std::shared_ptr<Point> p){
            return *p;
        });
        return std::make_unique<Polygon>(v);
    }
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: edges){
        std::unique_ptr<Point> inter = obj.intersect(*edge);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Polygon::intersect(const Polygon &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Point> p: vertices)
        if(obj.contains(*p)) points.push_back(*p);
    for(std::shared_ptr<Point> p: obj.vertices)
        if(contains(*p)) points.push_back(*p);
    if(glm::length2(glm::cross(normVec(), obj.normVec())) < 1e-5){
        for(std::shared_ptr<LinSeg> edge1: edges){
            for(std::shared_ptr<LinSeg> edge2: obj.edges){
                std::unique_ptr<Point> inter = edge1->intersect(*edge2);
                if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
            }
        }
    }
    else{
        for(std::shared_ptr<LinSeg> edge: edges){
            std::unique_ptr<Point> inter = obj.intersect(*edge);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
        for(std::shared_ptr<LinSeg> edge: obj.edges){
            std::unique_ptr<Point> inter = intersect(*edge);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
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
    for(std::shared_ptr<Point> p: vertices){
        if(obj.contains(*p)) points.push_back(*p);
    }
    for(std::shared_ptr<LinSeg> edge: edges){
        for(std::shared_ptr<Polygon> face: obj.faces){
            std::unique_ptr<Point> inter = edge->intersect(*face);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        std::unique_ptr<Point> inter = intersect(*edge);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
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
    for(std::shared_ptr<LinSeg> edge: edges) area += glm::length(glm::cross(edge->vertices[0]->pos - pos, edge->vertices[0]->pos - edge->vertices[1]->pos))/2;
    return area;
}

Polyhedron::Polyhedron(std::vector<Point> vert){
    vertices.resize(vert.size());
    std::transform(vert.begin(), vert.end(), vertices.begin(), [](Point p){
        return std::make_shared<Point>(p.pos);
    });
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: vertices) pos += p->pos;
    pos /= vertices.size();
    Point center{pos};
    std::vector<std::vector<std::shared_ptr<Point>>> points;
    for(unsigned int i = 3; i <= vertices.size(); i++){
        std::vector<std::vector<std::shared_ptr<Point>>> comb = combinations<std::shared_ptr<Point>>(vertices.begin(), vertices.end(), i);
        points.insert(points.end(), comb.begin(), comb.end());
    }
    for(std::reverse_iterator<std::vector<std::vector<std::shared_ptr<Point>>>::iterator> it = points.rbegin(); it != points.rend(); it++){
        try{
            std::shared_ptr<Polygon> face = std::make_shared<Polygon>(*it);
            if(face->sign_dist(center) < 0){
                std::swap((*it)[1], (*it)[2]);
                face = std::make_shared<Polygon>(*it);
            }
            bool side = std::all_of(vertices.begin(), vertices.end(), [&face](std::shared_ptr<Point> p){
                return face->sign_dist(*p) >= -1e-5;
            });
            bool subface = std::any_of(faces.begin(), faces.end(), [&face](std::shared_ptr<Polygon> f){
                return !face->contains(*f) && f->contains(*face);
            });
            if(side && !subface) faces.push_back(face);
        }
        catch(std::invalid_argument){}
    }
    if(faces.size() == 0) throw std::invalid_argument("Inputs cannot be coplanar");
    std::vector<std::shared_ptr<LinSeg>> lines;
    for(std::shared_ptr<Polygon> face: faces) lines.insert(lines.end(), face->edges.begin(), face->edges.end());
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(edges), [this](std::shared_ptr<LinSeg> l1){
        return std::find_if(edges.begin(), edges.end(), [&l1](std::shared_ptr<LinSeg> l2){return l1->equals(*l2);}) == edges.end();
    });
    if(faces.size() + vertices.size() - edges.size() != 2)
        throw std::invalid_argument("Inputs must define a convex polyhedron");
}

Polyhedron::Polyhedron(std::vector<std::shared_ptr<Point>> vert){
    vertices = vert;
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: vertices) pos += p->pos;
    pos /= vertices.size();
    Point center{pos};
    std::vector<std::vector<std::shared_ptr<Point>>> points;
    for(unsigned int i = 3; i < vertices.size(); i++){
        std::vector<std::vector<std::shared_ptr<Point>>> comb = combinations<std::shared_ptr<Point>>(vertices.begin(), vertices.end(), i);
        points.insert(points.end(), comb.begin(), comb.end());
    }
    for(std::reverse_iterator<std::vector<std::vector<std::shared_ptr<Point>>>::iterator> it = points.rbegin(); it != points.rend(); it++){
        try{
            std::shared_ptr<Polygon> face = std::make_shared<Polygon>(*it);
            if(face->sign_dist(center) < 0){
                std::swap((*it)[1], (*it)[2]);
                face = std::make_shared<Polygon>(*it);
            }
            bool side = std::all_of(vertices.begin(), vertices.end(), [&face](std::shared_ptr<Point> p){
                return face->sign_dist(*p) >= -1e-5;
            });
            bool subface = std::any_of(faces.begin(), faces.end(), [&face](std::shared_ptr<Polygon> f){
                return !face->contains(*f) && f->contains(*face);
            });
            if(side && !subface) faces.push_back(face);
        }
        catch(std::invalid_argument){}
    }
    if(faces.size() == 0) throw std::invalid_argument("Inputs cannot be coplanar");
    std::vector<std::shared_ptr<LinSeg>> lines;
    for(std::shared_ptr<Polygon> face: faces) lines.insert(lines.end(), face->edges.begin(), face->edges.end());
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(edges), [this](std::shared_ptr<LinSeg> l1){
        return std::find_if(edges.begin(), edges.end(), [&l1](std::shared_ptr<LinSeg> l2){return l1->equals(*l2);}) == edges.end();
    });
    if(faces.size() + vertices.size() - edges.size() != 2) throw std::invalid_argument("Inputs must define a convex polyhedron");
}

float Polyhedron::dist(const Point &obj) const {
    bool contained = std::all_of(faces.begin(), faces.end(), [&obj](std::shared_ptr<Polygon> face){
        return face->sign_dist(obj) >= 0;
    });
    if(contained) return 0;
    std::vector<float> distances(faces.size());
    std::transform(faces.begin(), faces.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const Line &obj) const {
    std::vector<float> distances(faces.size());
    std::transform(faces.begin(), faces.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const LinSeg &obj) const {
    for(std::shared_ptr<Point> p: obj.vertices) if(contains(*p)) return 0;
    std::vector<float> distances(faces.size());
    std::transform(faces.begin(), faces.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const Plane &obj) const {
    std::vector<float> distances(faces.size());
    std::transform(faces.begin(), faces.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}


float Polyhedron::dist(const Polygon &obj) const {
    for(std::shared_ptr<Point> p: obj.vertices) if(contains(*p)) return 0;
    std::vector<float> distances(faces.size());
    std::transform(faces.begin(), faces.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
        return face->dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}


float Polyhedron::dist(const Polyhedron &obj) const {
    for(std::shared_ptr<Point> p: obj.vertices) if(contains(*p)) return 0;
    std::vector<float> distances(faces.size());
    std::transform(faces.begin(), faces.end(), distances.begin(), [&obj](std::shared_ptr<Polygon> face){
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
    for(std::shared_ptr<Polygon> face: faces){
        std::unique_ptr<Point> inter = face->intersect(obj);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Polyhedron::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<Point> p: obj.vertices)
        if(contains(*p)) points.push_back(*p);
    for(std::shared_ptr<Polygon> face: faces){
        std::unique_ptr<Point> inter = face->intersect(obj);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point>::iterator it = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), it));
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points[0], points[1]);
}

std::unique_ptr<Point> Polyhedron::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    std::vector<Point> points;
    for(std::shared_ptr<LinSeg> edge: edges){
        std::unique_ptr<Point> inter = obj.intersect(*edge);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
    }
    std::vector<Point> out;
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
        for(std::shared_ptr<Polygon> face: faces){
            std::unique_ptr<Point> inter = edge->intersect(*face);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<LinSeg> edge: edges){
        std::unique_ptr<Point> inter = obj.intersect(*edge);
        if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
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
    for(std::shared_ptr<LinSeg> edge: edges){
        for(std::shared_ptr<Polygon> face: obj.faces){
            std::unique_ptr<Point> inter = edge->intersect(*face);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<LinSeg> edge: obj.edges){
        for(std::shared_ptr<Polygon> face: faces){
            std::unique_ptr<Point> inter = edge->intersect(*face);
            if(inter && typeid(*inter) == typeid(Point)) points.push_back(*inter);
        }
    }
    for(std::shared_ptr<Point> p: vertices){
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
    for(std::shared_ptr<Polygon> face: faces) vol += glm::dot(face->normVec(), pos - face->pos)*face->area()/3;
    return vol;
}
