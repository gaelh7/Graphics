#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>

class Point;
class Line;
class LinSeg;
class Plane;
class Polygon;
class Polyhedron;

class Point {
    public:
        std::vector<Point> vertices;
        glm::vec3 pos;
        Point();
        Point(glm::vec3 pos);
        virtual unsigned int dim() const {return 0;}
        virtual bool isSpace() const {return true;}
        virtual float dist(const Point &obj) const;
        virtual float dist(const Line &obj) const;
        virtual float dist(const LinSeg &obj) const;
        virtual float dist(const Plane &obj) const;
        virtual float dist(const Polygon &obj) const;
        virtual float dist(const Polyhedron &obj) const;
        virtual std::unique_ptr<Point> intersect(const Point &obj) const;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const;
        virtual std::unique_ptr<Point> intersect(const Polygon &obj) const;
        virtual std::unique_ptr<Point> intersect(const Polyhedron &obj) const;
        glm::vec3 direction(const Point &obj) const;
        bool contains(const Point &obj) const;
        bool equals(const Point &obj) const;
};

class Line: public Point {
    public:
        Line(){};
        Line(Point p1, Point p2);
        Line(std::vector<Point> vert);
        unsigned int dim() const override {return 1;}
        float dist(const Point &obj) const override;
        float dist(const Line &obj) const override;
        float dist(const LinSeg &obj) const override;
        float dist(const Plane &obj) const override;
        float dist(const Polygon &obj) const override;
        float dist(const Polyhedron &obj) const override;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        std::unique_ptr<Point> intersect(const Line &obj) const override;
        std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        std::unique_ptr<Point> intersect(const Plane &obj) const override;
        std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        glm::vec3 dirVec() const;
        std::unique_ptr<Point> project(const Point &obj) const;
        float angle(const Line &lobj, glm::vec3* axisptr);
};

class LinSeg: public Line {
    public:
        LinSeg(){};
        LinSeg(Point p1, Point p2);
        LinSeg(std::vector<Point> vert);
        bool isSpace() const override {return false;}
        float dist(const Point &obj) const override;
        float dist(const Line &obj) const override;
        float dist(const LinSeg &obj) const override;
        float dist(const Plane &obj) const override;
        float dist(const Polygon &obj) const override;
        float dist(const Polyhedron &obj) const override;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        std::unique_ptr<Point> intersect(const Line &obj) const override;
        std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        std::unique_ptr<Point> intersect(const Plane &obj) const override;
        std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        float length() const;
};

class Plane: public Point {
    public:
        Plane(){};
        Plane(Point p1, Point p2, Point p3);
        Plane(std::vector<Point> vert);
        unsigned int dim() const override {return 2;}
        glm::vec3 normVec() const;
        std::unique_ptr<Point> project(const Point &obj) const;
        template<typename T>
        std::unique_ptr<T> project(const T &obj) const {
            std::vector<Point> v;
            for(Point p: obj.vertices) v.push_back(*project(p));
            return std::unique_ptr<T>(new T(v));
        }
        float sign_dist(const Point &obj) const;
        float dist(const Point &obj) const override;
        float dist(const Line &obj) const override;
        float dist(const LinSeg &obj) const override;
        float dist(const Plane &obj) const override;
        float dist(const Polygon &obj) const override;
        float dist(const Polyhedron &obj) const override;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        std::unique_ptr<Point> intersect(const Line &obj) const override;
        std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        std::unique_ptr<Point> intersect(const Plane &obj) const override;
        std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
};

class Polygon: public Plane {
    public:
        std::vector<LinSeg> edges;
        Polygon(){};
        template <typename... Points>
        Polygon(Point p1, Point p2, Point p3, Points... args): Polygon(std::vector<Point>{p1, p2, p3, args...}){};
        Polygon(std::vector<Point> vert);
        bool isSpace() const override {return false;}
        float dist(const Point &obj) const override;
        float dist(const Line &obj) const override;
        float dist(const LinSeg &obj) const override;
        float dist(const Plane &obj) const override;
        float dist(const Polygon &obj) const override;
        float dist(const Polyhedron &obj) const override;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        std::unique_ptr<Point> intersect(const Line &obj) const override;
        std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        std::unique_ptr<Point> intersect(const Plane &obj) const override;
        std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        float area() const;
};

class Polyhedron: public Point {
    public:
        std::vector<LinSeg> edges;
        std::vector<Polygon> faces;
        Polyhedron(){};
        template <typename... Points>
        Polyhedron(Point p1, Point p2, Point p3, Point p4, Points... args): Polyhedron(std::vector<Point>{p1, p2, p3, p4, args...}){};
        Polyhedron(std::vector<Point> vert);
        unsigned int dim() const override {return 3;}
        bool isSpace() const override {return false;}
        float dist(const Point &obj) const override;
        float dist(const Line &obj) const override;
        float dist(const LinSeg &obj) const override;
        float dist(const Plane &obj) const override;
        float dist(const Polygon &obj) const override;
        float dist(const Polyhedron &obj) const override;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        // std::unique_ptr<Point> intersect(const Line &obj) const;
        // std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        // std::unique_ptr<Point> intersect(const Plane &obj) const;
        // std::unique_ptr<Point> intersect(const Polygon &obj) const;
        // std::unique_ptr<Point> intersect(const Polyhedron &obj) const;
        float volume() const;
};

static std::ostream& operator<<(std::ostream &strm, const Point &p){
    if(p.vertices.size() == 1)
        return strm << "Point(" << p.pos.x << ", " << p.pos.y << ", " << p.pos.z << ")";
    strm << typeid(p).name() << '(';
    for(unsigned int i = 0; i < p.vertices.size() - 1; i++)
        strm << p.vertices[i] << ", ";
    return strm << p.vertices[p.vertices.size() - 1] << ")";
};
