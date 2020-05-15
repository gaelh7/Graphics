#include <algorithm>
#include "Graphics/geometry.hpp"
#include "Graphics/gmath.hpp"
#include <glm/gtx/norm.hpp>

Point::Point(): pos({0, 0, 0}){
    vertices = {*this};
}

Point::Point(glm::vec3 pos): pos(pos){
    vertices = {*this};
}

float Point::dist(const Point &obj) const {
    return glm::distance(pos, obj.pos);
}

float Point::dist(const Line &obj) const {
    return glm::length(glm::cross(obj.vertices[0].pos - pos, obj.vertices[0].pos - obj.vertices[1].pos) / obj.vertices[0].dist(obj.vertices[1]));
}

float Point::dist(const LinSeg &obj) const {
    return glm::dot(pos - obj.vertices[0].pos, obj.vertices[1].pos - obj.vertices[0].pos) > 0 && glm::dot(pos - obj.vertices[1].pos, obj.vertices[0].pos - obj.vertices[1].pos) > 0 ? glm::length(glm::cross(obj.vertices[0].pos - pos, obj.vertices[0].pos - obj.vertices[1].pos) / obj.vertices[0].dist(obj.vertices[1])):std::min(dist(obj.vertices[0]), dist(obj.vertices[1]));
}

float Point::dist(const Plane &obj) const {
    return std::abs(glm::dot(obj.normVec(), pos - obj.vertices[0].pos));
}

float Point::dist(const Polygon &obj) const {
    for(LinSeg edge: obj.edges)
        if(glm::dot(glm::cross(edge.vertices[1].pos - edge.vertices[0].pos, pos - edge.vertices[0].pos), obj.normVec()) < 0){
            std::vector<float> distances;
            std::transform(obj.edges.begin(), obj.edges.end(), std::back_inserter(distances), [this](LinSeg lin){
                return dist(lin);
            });
            return *std::min_element(distances.begin(), distances.end());
        }
    return std::abs(glm::dot(obj.normVec(), pos - obj.vertices[0].pos));
}

float Point::dist(const Polyhedron &obj) const {
    bool contained = std::all_of(obj.faces.begin(), obj.faces.end(), [this](Polygon face){
        return face.sign_dist(*this) >= 0;
    });
    if(contained) return 0;
    std::vector<float> distances;
    std::transform(obj.faces.begin(), obj.faces.end(), std::back_inserter(distances), [this](Polygon face){
        return face.dist(*this);
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
    return std::none_of(obj.vertices.begin(), obj.vertices.end(), [this](Point vert){
        return dist(vert) >= 1e-5;
    });
}

bool Point::equals(const Point &obj) const {
    return obj.contains(*this) && contains(obj);
}

Line::Line(Point p1, Point p2){
    if(p1.equals(p2))
        throw std::invalid_argument("Inputs must have different positions");
    vertices = {p1, p2};
    pvertices = {std::make_shared<Point>(p1.pos), std::make_shared<Point>(p1.pos)};
}

Line::Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
    if(p1->equals(*p2))
        throw std::invalid_argument("Inputs must have different positions");
    pvertices = {p1, p2};
}

Line::Line(std::vector<Point> vert): Line(vert[0], vert[1]){}

Line::Line(std::vector<std::shared_ptr<Point>> vert): Line(vert[0], vert[1]){}

float Line::dist(const Point &obj) const {
    return glm::length(glm::cross(vertices[0].pos - obj.pos, vertices[0].pos - vertices[1].pos) / vertices[0].dist(vertices[1]));
}

float Line::dist(const Line &obj) const {
    glm::vec3 vec = glm::cross(dirVec(), obj.dirVec());
    return glm::length2(vec) < 1e-5 ? dist(obj.vertices[0]):std::abs(glm::dot(vec, vertices[0].pos - obj.vertices[1].pos))/glm::length(vec);
}

float Line::dist(const LinSeg &obj) const {
    glm::vec3 c = glm::cross(dirVec(), obj.dirVec());
    if(glm::length2(c) < 1e-5) return dist(obj.vertices[0]);
    float t = det(obj.vertices[0].pos - vertices[0].pos, dirVec(), c)/glm::length2(c);
    return t < 0 || t > glm::distance(obj.vertices[1].pos, obj.vertices[0].pos) ? std::min(dist(obj.vertices[0]), dist(obj.vertices[1])):dist((Line)obj);
}

float Line::dist(const Plane &obj) const {
    return std::abs(glm::dot(obj.normVec(), dirVec())) < 1e-5 ? obj.dist(vertices[0]):0;
}

float Line::dist(const Polygon &obj) const {
    Plane pl(obj.vertices[0], obj.vertices[1], obj.vertices[2]);
    std::unique_ptr<Point> p = pl.intersect(*this);
    if(p != nullptr && obj.contains(*p)) return 0;
    std::vector<float> distances;
    std::transform(obj.edges.begin(), obj.edges.end(), std::back_inserter(distances), [this](LinSeg lin){
        return dist(lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Line::dist(const Polyhedron &obj) const {
    std::vector<float> distances;
    std::transform(obj.faces.begin(), obj.faces.end(), std::back_inserter(distances), [this](Polygon face){
        return face.dist(*this);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Line::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> Line::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return obj.isSpace() ? std::make_unique<Line>(obj.vertices):obj.intersect(*this);
    else if(contains(obj.vertices[0])) return std::make_unique<Point>(obj.vertices[0]);
    else if(obj.contains(vertices[0])) return std::make_unique<Point>(vertices[0]);
    glm::vec3 vec1 = glm::cross(obj.dirVec(), obj.vertices[0].pos - vertices[0].pos);
    glm::vec3 vec2 = glm::cross(obj.dirVec(), dirVec());
    return std::make_unique<Point>(vertices[0].pos + (sign(glm::dot(vec1, vec2)))*(glm::length(vec1)/glm::length(vec2))*dirVec());
}

std::unique_ptr<Point> Line::intersect(const LinSeg &obj) const {
    return contains(obj) ? std::make_unique<LinSeg>(obj.vertices):intersect((Line)obj);
}

std::unique_ptr<Point> Line::intersect(const Plane &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    if(obj.contains(*this)) return std::make_unique<Line>(this->vertices);
    if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0].pos - obj.normVec()*obj.sign_dist(vertices[0]));
    return intersect(*obj.project(*this));
}

std::unique_ptr<Point> Line::intersect(const Polygon &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0].pos - obj.normVec()*obj.sign_dist(vertices[0]));
    if(std::abs(glm::dot(dirVec(), obj.normVec())) < 1e-5){
        std::vector<Point> points;
        for(LinSeg edge: obj.edges){
            std::unique_ptr<Point> inter = intersect(edge);
            if(inter != nullptr && typeid(*inter) == typeid(vertices[0])) points.push_back(*inter);
        }
        std::vector<Point>::iterator p = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), p));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points);
    }
    return intersect(*project(*this));
}

std::unique_ptr<Point> Line::intersect(const Polyhedron &obj) const {
    // TODO: Implement this
    return nullptr;
}

glm::vec3 Line::dirVec() const {
    return vertices[0].direction(vertices[1]);
}

std::unique_ptr<Point> Line::project(const Point &obj) const {
    return std::make_unique<Point>(vertices[0].pos + dirVec()*glm::dot(obj.pos - vertices[0].pos, dirVec()));
}

float Line::angle(const Line &lobj, glm::vec3* axisptr = nullptr){
    glm::vec3 axis = axisptr == nullptr ? glm::cross(dirVec(), lobj.dirVec()):*axisptr;
    axis = glm::normalize(axis);
    float theta{std::atan2(det(dirVec(), lobj.dirVec(), axis), glm::dot(dirVec(), lobj.dirVec()))};
    return theta >= 0 ? theta:theta + 2*glm::pi<float>();
}

LinSeg::LinSeg(Point p1, Point p2): Line(p1,p2){}

LinSeg::LinSeg(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2): Line(p1,p2){}

LinSeg::LinSeg(std::vector<Point> vert): Line(vert[0], vert[1]){}

LinSeg::LinSeg(std::vector<std::shared_ptr<Point>> vert): Line(vert[0], vert[1]){}

float LinSeg::dist(const Point &obj) const {
    return glm::dot(obj.pos - vertices[0].pos, vertices[1].pos - vertices[0].pos) > 0 && glm::dot(obj.pos - vertices[1].pos, vertices[0].pos - vertices[1].pos) > 0 ? glm::length(glm::cross(vertices[0].pos - obj.pos, vertices[0].pos - vertices[1].pos) / vertices[0].dist(vertices[1])):std::min(obj.dist(vertices[0]), obj.dist(vertices[1]));
}

float LinSeg::dist(const Line &obj) const {
    glm::vec3 c = glm::cross(obj.dirVec(), dirVec());
    if(glm::length2(c) < 1e-5) return obj.dist(vertices[0]);
    float t = det(vertices[0].pos - obj.vertices[0].pos, obj.dirVec(), c)/glm::length2(c);
    return t < 0 || t > glm::distance(vertices[1].pos, vertices[0].pos) ? std::min(obj.dist(vertices[0]), obj.dist(vertices[1])):obj.dist((Line)*this);
}

float LinSeg::dist(const LinSeg &obj) const {
    glm::vec3 c = glm::cross(dirVec(), obj.dirVec());
    glm::vec3 t = obj.vertices[0].pos - vertices[0].pos;
    float c_squared = glm::length2(c);
    float t0 = det(t, obj.dirVec(), c)/c_squared;
    float t1 = det(t, dirVec(), c)/c_squared;
    if(glm::length2(c) < 1e-5 || t0 < 0 || t0 > length()) return std::min(obj.dist(vertices[0]), obj.dist(vertices[1]));
    else if(t1 < 0 || t1 > obj.length()) return std::min(dist(obj.vertices[0]), dist(obj.vertices[1]));
    return std::abs(glm::dot(c, vertices[0].pos - obj.vertices[1].pos))/glm::length(c);
}

float LinSeg::dist(const Plane &obj) const {
    if((sign(obj.sign_dist(vertices[0])) ^ sign(obj.sign_dist(vertices[1]))) < 0) return 0;
    return std::min(obj.dist(vertices[0]), obj.dist(vertices[1]));
}

float LinSeg::dist(const Polygon &obj) const {
    Plane pl(obj.vertices[0], obj.vertices[1], obj.vertices[2]);
    std::unique_ptr<Point> p = pl.intersect(*this);
    if(p != nullptr && contains(*p)) return 0;
    std::vector<float> distances;
    std::transform(obj.edges.begin(), obj.edges.end(), std::back_inserter(distances), [this](LinSeg lin){
        return dist(lin);
    });
    float min = *std::min_element(distances.begin(), distances.end());
    min = std::min(min, obj.dist(vertices[0]));
    min = std::min(min, obj.dist(vertices[1]));
    return min;
}

float LinSeg::dist(const Polyhedron &obj) const {
    for(Point p: vertices) if(obj.contains(p)) return 0;
    std::vector<float> distances;
    std::transform(obj.faces.begin(), obj.faces.end(), std::back_inserter(distances), [this](Polygon face){
        return face.dist(*this);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> LinSeg::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> LinSeg::intersect(const Line &obj) const {
    return obj.contains(*this) ? std::make_unique<LinSeg>(this->vertices):obj.intersect(*this);
}

std::unique_ptr<Point> LinSeg::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<LinSeg>(obj.vertices);
    else if(obj.contains(*this)) return std::make_unique<LinSeg>(this->vertices);
    if(glm::length2(glm::cross(dirVec(), obj.dirVec())) < 1e-5){
        if(obj.contains(vertices[0])){
            if(vertices[0].equals(obj.vertices[0]) || vertices[0].equals(obj.vertices[1])) return std::make_unique<Point>(vertices[0]);
            return contains(obj.vertices[0]) ? std::make_unique<LinSeg>(vertices[0], obj.vertices[0]):std::make_unique<LinSeg>(vertices[0], obj.vertices[1]);
        }
        if(obj.contains(vertices[1])){
            if(vertices[1].equals(obj.vertices[0]) || vertices[1].equals(obj.vertices[1])) return std::make_unique<Point>(vertices[1]);
            return contains(obj.vertices[0]) ? std::make_unique<LinSeg>(vertices[1], obj.vertices[0]):std::make_unique<LinSeg>(vertices[1], obj.vertices[1]);
        }
    }
    return Line::intersect(obj);
}

std::unique_ptr<Point> LinSeg::intersect(const Plane &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    if(obj.contains(*this)) return std::make_unique<LinSeg>(this->vertices);
    if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0].pos - obj.normVec()*obj.sign_dist(vertices[0]));
    return intersect(*obj.project(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const Polygon &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(dirVec(), obj.normVec())) < 1e-5) return std::make_unique<Point>(vertices[0].pos - obj.normVec()*obj.sign_dist(vertices[0]));
    else if(obj.contains(*this)) return std::make_unique<LinSeg>(this->vertices);
    if(std::abs(glm::dot(dirVec(), obj.normVec())) < 1e-5){
        std::vector<Point> points;
        if(obj.contains(vertices[0])) points.push_back(vertices[0]);
        else if(contains(vertices[1])) points.push_back(vertices[1]);
        for(LinSeg edge: obj.edges){
            std::unique_ptr<Point> inter = intersect(edge);
            if(inter != nullptr && typeid(*inter) == typeid(vertices[0])) points.push_back(*inter);
        }
        std::vector<Point>::iterator p = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), p));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points);
    }
    return intersect(*obj.project(*this));
}

std::unique_ptr<Point> LinSeg::intersect(const Polyhedron &obj) const {
    // TODO: Implement this
    return nullptr;
}

float LinSeg::length() const {
    return glm::distance(vertices[0].pos, vertices[1].pos);
}

Plane::Plane(Point p1, Point p2, Point p3){
    if(Line(p1, p2).contains(p3)) throw std::invalid_argument("Inputs cannot be collinear");
    vertices = {p1, p2, p3};
    pvertices = {std::make_shared<Point>(p1.pos), std::make_shared<Point>(p2.pos), std::make_shared<Point>(p3.pos)};
}

Plane::Plane(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<Point> p3){
    if(Line(*p1, *p2).contains(*p3)) throw std::invalid_argument("Inputs cannot be collinear");
    pvertices = {p1, p2, p3};
}

Plane::Plane(std::vector<Point> vert): Plane(vert[0], vert[1], vert[2]){}

Plane::Plane(std::vector<std::shared_ptr<Point>> vert): Plane(vert[0], vert[1], vert[2]){}

glm::vec3 Plane::normVec() const {
    glm::vec3 vec = glm::cross(vertices[1].pos - vertices[0].pos, vertices[2].pos - vertices[0].pos);
    return glm::normalize(vec);
}

std::unique_ptr<Point> Plane::project(const Point &obj) const {
    if(obj.dim() == 3) throw std::invalid_argument("Input dimensions must be less than or equal to two");
    return std::make_unique<Point>(obj.pos - normVec()*sign_dist(obj));
}

float Plane::sign_dist(const Point &obj) const {
    return glm::dot(normVec(), obj.pos - vertices[0].pos);
}

float Plane::dist(const Point &obj) const {
    return std::abs(glm::dot(normVec(), obj.pos - vertices[0].pos));
}

float Plane::dist(const Line &obj) const {
    return std::abs(glm::dot(normVec(), obj.dirVec())) < 1e-5 ? dist(obj.vertices[0]):0;
}

float Plane::dist(const LinSeg &obj) const {
    if((sign(sign_dist(obj.vertices[0])) ^ sign(sign_dist(obj.vertices[1]))) < 0) return 0;
    return std::min(dist(obj.vertices[0]), dist(obj.vertices[1]));
}

float Plane::dist(const Plane &obj) const {
    return glm::length2(glm::cross(normVec(), obj.normVec())) < 1e-5 ? dist(obj.vertices[0]):0;
}

float Plane::dist(const Polygon &obj) const {
    std::vector<float> distances;
    std::transform(obj.edges.begin(), obj.edges.end(), std::back_inserter(distances), [this](LinSeg lin){
        return dist(lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Plane::dist(const Polyhedron &obj) const {
    std::vector<float> distances;
    std::transform(obj.faces.begin(), obj.faces.end(), std::back_inserter(distances), [this](Polygon face){
        return face.dist(*this);
    });
    return *std::min_element(distances.begin(), distances.end());
};

std::unique_ptr<Point> Plane::intersect(const Point &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> Plane::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<Line>(obj.vertices);
    if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0].pos - normVec()*sign_dist(obj.vertices[0]));
    return obj.intersect(*project(obj));
}

std::unique_ptr<Point> Plane::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    if(contains(obj)) return std::make_unique<LinSeg>(obj.vertices);
    if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0].pos - normVec()*sign_dist(obj.vertices[0]));
    return obj.intersect(*project(obj));
}

std::unique_ptr<Point> Plane::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(contains(obj)) return std::make_unique<Plane>(obj.vertices);
    std::unique_ptr<Point> x = obj.intersect(Line(vertices[0], vertices[1]));
    if(x == nullptr) x = obj.intersect(Line(vertices[0], vertices[2]));
    return std::make_unique<Line>(*x, Point(x->pos + glm::cross(normVec(), obj.normVec())));
}

std::unique_ptr<Point> Plane::intersect(const Polygon &obj) const {
    if(obj.dist(*this) >= 1e-5) return nullptr;
    else if(contains(obj)) return std::make_unique<Polygon>(obj.vertices);
    std::vector<Point> points;
    for(LinSeg edge: obj.edges){
        std::unique_ptr<Point> inter = intersect(edge);
        if(inter != nullptr && typeid(*inter) == typeid(vertices[0])) points.push_back(*inter);
    }
    std::vector<Point>::iterator p = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), p));
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points);
}

std::unique_ptr<Point> Plane::intersect(const Polyhedron &obj) const {
    // TODO: Implement this
    return nullptr;
}

Polygon::Polygon(std::vector<Point> vert): Plane(vert[0], vert[1], vert[2]){
    vertices = vert;
    pvertices.resize(vert.size());
    std::transform(vert.begin(), vert.end(), pvertices.begin(), [](Point p){
        return std::make_shared<Point>(p.pos);
    });
    pos = {0, 0, 0};
    for(Point p: vertices) pos += p.pos;
    pos /= vertices.size();
    Point center{pos};
    Line l{center, vertices[0]};
    glm::vec3 vec = glm::cross(vertices[0].direction(vertices[1]), vertices[0].direction(vertices[2]));
    std::sort(vertices.begin(), vertices.end(), [&l, &vec, &center](Point p1, Point p2){
        return l.angle(Line(center, p1), &vec) < l.angle(Line(center, p2), &vec);
    });
    for(unsigned int i = 0; i < vertices.size(); i++){
        edges.push_back(LinSeg(vertices[i], vertices[(i+1)%vertices.size()]));
        if(i > 0 && edges[i].dirVec() == edges[i-1].dirVec() || i == vertices.size() - 1 && edges[i].dirVec() == edges[0].dirVec())
            throw std::invalid_argument("Inputs cannot be collinear");
        if(i > 0 && edges[i-1].angle(edges[i%edges.size()], &vec) > glm::pi<float>() || i == vertices.size() - 1 && edges[i].angle(edges[0], &vec) > glm::pi<float>())
            throw std::invalid_argument("Inputs must define a convex polygon");
    }
}

Polygon::Polygon(std::vector<std::shared_ptr<Point>> vert){
    pvertices = vert;
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: pvertices) pos += p->pos;
    pos /= pvertices.size();
    Point center{pos};
    Line l{center, *pvertices[0]};
    glm::vec3 vec = glm::cross(pvertices[0]->direction(*pvertices[1]), pvertices[0]->direction(*pvertices[2]));
    std::sort(pvertices.begin(), pvertices.end(), [&l, &vec, &center](std::shared_ptr<Point> p1, std::shared_ptr<Point> p2){
        return l.angle(Line(center, *p1), &vec) < l.angle(Line(center, *p2), &vec);
    });
    for(unsigned int i = 0; i < pvertices.size(); i++){
        pedges.push_back(std::make_shared<LinSeg>(pvertices[i], pvertices[(i+1)%pvertices.size()]));
        if(i > 0 && pedges[i]->dirVec() == pedges[i-1]->dirVec() || i == pvertices.size() - 1 && pedges[i]->dirVec() == pedges[0]->dirVec())
            throw std::invalid_argument("Inputs cannot be collinear");
        if(i > 0 && pedges[i-1]->angle(*pedges[i%pedges.size()], &vec) > glm::pi<float>() || i == pvertices.size() - 1 && pedges[i]->angle(*pedges[0], &vec) > glm::pi<float>())
            throw std::invalid_argument("Inputs must define a convex polygon");
    }
}

float Polygon::dist(const Point &obj) const {
    for(LinSeg edge: edges)
        if(glm::dot(glm::cross(edge.vertices[1].pos - edge.vertices[0].pos, obj.pos - edge.vertices[0].pos), normVec()) < 0){
            std::vector<float> distances;
            std::transform(edges.begin(), edges.end(), std::back_inserter(distances), [&obj](LinSeg lin){
                return obj.dist(lin);
            });
            return *std::min_element(distances.begin(), distances.end());
        }
    return std::abs(glm::dot(normVec(), obj.pos - vertices[0].pos));
}

float Polygon::dist(const Line &obj) const {
    Plane pl(vertices[0],vertices[1],vertices[2]);
    std::unique_ptr<Point> p = pl.intersect(obj);
    if(p != nullptr && contains(*p)) return 0;
    std::vector<float> distances;
    std::transform(edges.begin(), edges.end(), std::back_inserter(distances), [&obj](LinSeg lin){
        return obj.dist(lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polygon::dist(const LinSeg &obj) const {
    Plane pl(vertices[0],vertices[1],vertices[2]);
    std::unique_ptr<Point> p = pl.intersect(obj);
    if(p != nullptr && contains(*p)) return 0;
    std::vector<float> distances;
    std::transform(edges.begin(), edges.end(), std::back_inserter(distances), [&obj](LinSeg lin){
        return obj.dist(lin);
    });
    float min = *std::min_element(distances.begin(), distances.end());
    min = std::min(min, dist(obj.vertices[0]));
    min = std::min(min, dist(obj.vertices[1]));
    return min;
}

float Polygon::dist(const Plane &obj) const {
    std::vector<float> distances;
    std::transform(edges.begin(), edges.end(), std::back_inserter(distances), [&obj](LinSeg lin){
        return obj.dist(lin);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polygon::dist(const Polygon &obj) const {
    std::vector<float> dist1;
    std::vector<float> dist2;
    std::transform(edges.begin(), edges.end(), std::back_inserter(dist1), [&obj](LinSeg lin){return obj.dist(lin);});
    std::transform(obj.edges.begin(), obj.edges.end(), std::back_inserter(dist2), [this](LinSeg lin){return dist(lin);});
    return std::min(*std::min_element(dist1.begin(), dist1.end()), *std::min_element(dist2.begin(), dist2.end()));
}

float Polygon::dist(const Polyhedron &obj) const {
    for(Point p: vertices) if(obj.contains(p)) return 0;
    std::vector<float> distances;
    std::transform(obj.faces.begin(), obj.faces.end(), std::back_inserter(distances), [this](Polygon face){
        return face.dist(*this);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Polygon::intersect(const Point &obj) const{
    if(dist(obj) >= 1e-5) return nullptr;
    return std::make_unique<Point>(obj.pos);
}

std::unique_ptr<Point> Polygon::intersect(const Line &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0].pos - normVec()*sign_dist(obj.vertices[0]));
    if(std::abs(glm::dot(obj.dirVec(), normVec())) < 1e-5){
        std::vector<Point> points;
        for(LinSeg edge: edges){
            std::unique_ptr<Point> inter = obj.intersect(edge);
            if(inter != nullptr && typeid(*inter) == typeid(obj.vertices[0])) points.push_back(*inter);
        }
        std::vector<Point>::iterator p = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), p));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points);
    }
    return obj.intersect(*project(obj));
}

std::unique_ptr<Point> Polygon::intersect(const LinSeg &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(glm::length2(glm::cross(obj.dirVec(), normVec())) < 1e-5) return std::make_unique<Point>(obj.vertices[0].pos - normVec()*sign_dist(obj.vertices[0]));
    else if(contains(obj)) return std::make_unique<LinSeg>(obj.vertices);
    if(std::abs(glm::dot(obj.dirVec(), normVec())) < 1e-5){
        std::vector<Point> points;
        if(contains(obj.vertices[0])) points.push_back(obj.vertices[0]);
        else if(contains(obj.vertices[1])) points.push_back(obj.vertices[1]);
        for(LinSeg edge: edges){
            std::unique_ptr<Point> inter = obj.intersect(edge);
            if(inter != nullptr && typeid(*inter) == typeid(obj.vertices[0])) points.push_back(*inter);
        }
        std::vector<Point>::iterator p = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
            return p1.equals(p2);
        });
        points.resize(std::distance(points.begin(), p));
        return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points);
    }
    return obj.intersect(*project(obj));
}

std::unique_ptr<Point> Polygon::intersect(const Plane &obj) const {
    if(dist(obj) >= 1e-5) return nullptr;
    else if(obj.contains(*this)) return std::make_unique<Polygon>(this->vertices);
    std::vector<Point> points;
    for(LinSeg edge: edges){
        std::unique_ptr<Point> inter = obj.intersect(edge);
        if(inter != nullptr && typeid(*inter) == typeid(obj.vertices[0])) points.push_back(*inter);
    }
    std::vector<Point>::iterator p = std::unique(points.begin(), points.end(), [](Point p1, Point p2){
        return p1.equals(p2);
    });
    points.resize(std::distance(points.begin(), p));
    return points.size() == 1 ? std::make_unique<Point>(points[0]):std::make_unique<LinSeg>(points);
}

std::unique_ptr<Point> Polygon::intersect(const Polygon &obj) const {return std::make_unique<Point>();}

std::unique_ptr<Point> Polygon::intersect(const Polyhedron &obj) const {
    // TODO: This
    return nullptr;
}

float Polygon::area() const {
    float area = 0;
    for(LinSeg edge: edges) area += glm::length(glm::cross(edge.vertices[0].pos - pos, edge.vertices[0].pos - edge.vertices[1].pos))/2;
    return area;
}

Polyhedron::Polyhedron(std::vector<Point> vert){
    vertices = vert;
    pvertices.resize(vert.size());
    std::transform(vert.begin(), vert.end(), pvertices.begin(), [](Point p){
        return std::make_shared<Point>(p.pos);
    });
    pos = {0, 0, 0};
    for(Point p: vertices) pos += p.pos;
    pos /= vertices.size();
    Point center{pos};
    std::vector<std::vector<Point>> points;
    for(unsigned int i = 3; i < vertices.size(); i++){
        std::vector<std::vector<Point>> comb = combinations<Point>(vertices.begin(), vertices.end(), i);
        points.insert(points.end(), comb.begin(), comb.end());
    }
    for(std::vector<Point> v: points){
        try{
            Polygon face = Polygon(v);
            if(face.sign_dist(center) < 0){
                std::swap(v[1], v[2]);
                face = Polygon(v);
            }
            std::vector<Point> vert_check;
            std::copy_if(vertices.begin(), vertices.end(), std::back_inserter(vert_check), [&face](Point p){
                return !face.contains(p);
            });
            bool side = std::all_of(vert_check.begin(), vert_check.end(), [&face](Point p){
                return face.sign_dist(p) > 0;
            });
            bool subface = std::any_of(faces.begin(), faces.end(), [&face](Polygon f){
                return face.contains(f) && !f.contains(face);
            });
            if(side && !subface) faces.push_back(face);
        }
        catch(std::invalid_argument){}
    }
    if(faces.size() == 0) throw std::invalid_argument("Inputs cannot be coplanar");
    std::vector<LinSeg> lines;
    for(Polygon face: faces) lines.insert(lines.end(), face.edges.begin(), face.edges.end());
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(edges), [this](LinSeg l1){
        return std::find_if(edges.begin(), edges.end(), [&l1](LinSeg l2){return l1.equals(l2);}) == edges.end();
    });
    if(faces.size() + vertices.size() - edges.size() != 2) throw std::invalid_argument("Inputs must define a convex polyhedron");
}

Polyhedron::Polyhedron(std::vector<std::shared_ptr<Point>> vert){
    pvertices = vert;
    pos = {0, 0, 0};
    for(std::shared_ptr<Point> p: pvertices) pos += p->pos;
    pos /= pvertices.size();
    Point center{pos};
    std::vector<std::vector<std::shared_ptr<Point>>> points;
    for(unsigned int i = 3; i < pvertices.size(); i++){
        std::vector<std::vector<std::shared_ptr<Point>>> comb = combinations<std::shared_ptr<Point>>(pvertices.begin(), pvertices.end(), i);
        points.insert(points.end(), comb.begin(), comb.end());
    }
    for(std::vector<std::shared_ptr<Point>> v: points){
        try{
            std::shared_ptr<Polygon> face = std::make_shared<Polygon>(v);
            if(face->sign_dist(center) < 0){
                std::swap(v[1], v[2]);
                face = std::make_shared<Polygon>(v);
            }
            std::vector<std::shared_ptr<Point>> vert_check;
            std::copy_if(pvertices.begin(), pvertices.end(), std::back_inserter(vert_check), [&face](std::shared_ptr<Point> p){
                return !face->contains(*p);
            });
            bool side = std::all_of(vert_check.begin(), vert_check.end(), [&face](std::shared_ptr<Point> p){
                return face->sign_dist(*p) > 0;
            });
            bool subface = std::any_of(pfaces.begin(), pfaces.end(), [&face](std::shared_ptr<Polygon> f){
                return face->contains(*f) && !f->contains(*face);
            });
            if(side && !subface) pfaces.push_back(face);
        }
        catch(std::invalid_argument){}
    }
    if(pfaces.size() == 0) throw std::invalid_argument("Inputs cannot be coplanar");
    std::vector<std::shared_ptr<LinSeg>> lines;
    for(std::shared_ptr<Polygon> face: pfaces) lines.insert(lines.end(), face->pedges.begin(), face->pedges.end());
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(pedges), [this](std::shared_ptr<LinSeg> l1){
        return std::find_if(pedges.begin(), pedges.end(), [&l1](std::shared_ptr<LinSeg> l2){return l1->equals(*l2);}) == pedges.end();
    });
    if(pfaces.size() + pvertices.size() - pedges.size() != 2) throw std::invalid_argument("Inputs must define a convex polyhedron");
}

float Polyhedron::dist(const Point &obj) const {
    bool contained = std::all_of(faces.begin(), faces.end(), [&obj](Polygon face){
        return face.sign_dist(obj) >= 0;
    });
    if(contained) return 0;
    std::vector<float> distances;
    std::transform(faces.begin(), faces.end(), std::back_inserter(distances), [&obj](Polygon face){
        return face.dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const Line &obj) const {
    std::vector<float> distances;
    std::transform(faces.begin(), faces.end(), std::back_inserter(distances), [&obj](Polygon face){
        return face.dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const LinSeg &obj) const {
    for(Point p: obj.vertices) if(contains(p)) return 0;
    std::vector<float> distances;
    std::transform(faces.begin(), faces.end(), std::back_inserter(distances), [&obj](Polygon face){
        return face.dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

float Polyhedron::dist(const Plane &obj) const {
    std::vector<float> distances;
    std::transform(faces.begin(), faces.end(), std::back_inserter(distances), [&obj](Polygon face){
        return face.dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}


float Polyhedron::dist(const Polygon &obj) const {
    for(Point p: obj.vertices) if(contains(p)) return 0;
    std::vector<float> distances;
    std::transform(faces.begin(), faces.end(), std::back_inserter(distances), [&obj](Polygon face){
        return face.dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}


float Polyhedron::dist(const Polyhedron &obj) const {
    for(Point p: obj.vertices) if(contains(p)) return 0;
    std::vector<float> distances;
    std::transform(faces.begin(), faces.end(), std::back_inserter(distances), [&obj](Polygon face){
        return face.dist(obj);
    });
    return *std::min_element(distances.begin(), distances.end());
}

std::unique_ptr<Point> Polyhedron::intersect(const Point &obj) const {
    return dist(obj) < 1e-5 ? std::make_unique<Point>(obj.pos):nullptr;
}

float Polyhedron::volume() const {
    float vol = 0;
    for(Polygon face: faces) vol += glm::dot(face.normVec(), pos - face.pos)*face.area()/3;
    return vol;
}
