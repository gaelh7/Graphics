#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <glm/ext/vector_float3.hpp>

namespace gmh{

/**
 * @brief Defines a point in 3D space.
 *
 * Used as a building block to construct more complex
 * shapes (Line, Polygon, etc). Can be passed to
 * those objects by value, or through a shared pointer.
 * Passing by value will allow objects to copy it,
 * passing by pointer will have the objects store a
 * shared pointer to the same point.
 */
class Point;

/**
 * Defines a line in 3D space from two points objects.
 */
class Line;

/**
 * Defines a line segment in 3D space from two points.
 */
class LinSeg;

/**
 * Defines a plane in 3D space from three points.
 */
class Plane;

/**
 * @brief Define a convex polygon in 3D space from any number of given points.
 *
 * The order in which the points are stored is counterclockwise
 * around the first input. Counterclockwise is defined by the
 * Polygon's normal vector as defined by the first three input points.
 * Polygon's also store a vector of pointers to line segments going
 * counterclockwise. The line segments vertices are pointers to the same
 * in the polygon.
 */
class Polygon;

/**
 * @brief Define a convex polyhedron from any number of given points.
 *
 * Unlike Polygons, the vertices are stored in no particular order,
 * but are used to define a set of LinSeg and Polygon objects
 * which are used for certain calculations.
 */
class Polyhedron;

class Point {
    public:
        glm::vec3 pos;
        glm::vec3 vel;
        glm::mat4 model;
        std::vector<std::shared_ptr<Point>> vertices;
        Point();
        Point(glm::vec3 pos);
        inline virtual unsigned int dim() const {return 0;}
        inline virtual bool isSpace() const {return true;}
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
        void update(float dt);
};

class Line: public Point {
    public:
        Line();
        Line(Point p1, Point p2);
        Line(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
        Line(std::vector<Point> vert);
        Line(std::vector<std::shared_ptr<Point>> vert);
        inline virtual unsigned int dim() const override {return 1;}
        virtual float dist(const Point &obj) const override;
        virtual float dist(const Line &obj) const override;
        virtual float dist(const LinSeg &obj) const override;
        virtual float dist(const Plane &obj) const override;
        virtual float dist(const Polygon &obj) const override;
        virtual float dist(const Polyhedron &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const override;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        glm::vec3 dirVec() const;
        Point project(const Point &obj) const;
        float angle(const Line &lobj, glm::vec3* axisptr = nullptr);
};

class LinSeg: public Line {
    public:
        LinSeg();
        LinSeg(Point p1, Point p2);
        LinSeg(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
        LinSeg(std::vector<Point> vert);
        LinSeg(std::vector<std::shared_ptr<Point>> vert);
        inline virtual bool isSpace() const override {return false;}
        virtual float dist(const Point &obj) const override;
        virtual float dist(const Line &obj) const override;
        virtual float dist(const LinSeg &obj) const override;
        virtual float dist(const Plane &obj) const override;
        virtual float dist(const Polygon &obj) const override;
        virtual float dist(const Polyhedron &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const override;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        float length() const;
};

class Plane: public Point {
    public:
        Plane();
        Plane(Point p1, Point p2, Point p3);
        Plane(std::vector<Point> vert);
        Plane(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<Point> p3);
        Plane(std::vector<std::shared_ptr<Point>> vert);
        inline virtual unsigned int dim() const override {return 2;}
        glm::vec3 normVec() const;
        Point project(const Point &obj) const;
        template<typename T>
        T project(const T &obj) const {
            std::vector<Point> v(obj.vertices.size());
            std::transform(obj.vertices.begin(), obj.vertices.end(), v.begin(), [this](std::shared_ptr<Point> p){
                return project(*p);
            });
            return T(v);
        }
        float sign_dist(const Point &obj) const;
        virtual float dist(const Point &obj) const override;
        virtual float dist(const Line &obj) const override;
        virtual float dist(const LinSeg &obj) const override;
        virtual float dist(const Plane &obj) const override;
        virtual float dist(const Polygon &obj) const override;
        virtual float dist(const Polyhedron &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const override;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
};

class Polygon: public Plane {
    public:
        std::vector<std::shared_ptr<LinSeg>> edges;
        Polygon();
        template <typename... Points>
        Polygon(Point p1, Point p2, Point p3, Points... args): Polygon(std::vector<Point>{p1, p2, p3, args...}){};
        Polygon(std::vector<Point> vert);
        template <typename... Points>
        Polygon(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<Point> p3, Points... args): Polygon(std::vector<std::shared_ptr<Point>>{p1, p2, p3, args...}){};
        Polygon(std::vector<std::shared_ptr<Point>> vert);
        inline virtual bool isSpace() const override {return false;}
        virtual float dist(const Point &obj) const override;
        virtual float dist(const Line &obj) const override;
        virtual float dist(const LinSeg &obj) const override;
        virtual float dist(const Plane &obj) const override;
        virtual float dist(const Polygon &obj) const override;
        virtual float dist(const Polyhedron &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const override;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        float area() const;
};

class Polyhedron: public Point {
    public:
        std::vector<std::shared_ptr<LinSeg>> edges;
        std::vector<std::shared_ptr<Polygon>> faces;
        Polyhedron();
        template <typename... Points>
        Polyhedron(Point p1, Point p2, Point p3, Point p4, Points... args): Polyhedron(std::vector<Point>{p1, p2, p3, p4, args...}){};
        Polyhedron(std::vector<Point> vert);
        template <typename... Points>
        Polyhedron(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, std::shared_ptr<Point> p3, std::shared_ptr<Point> p4, Points... args): Polyhedron(std::vector<std::shared_ptr<Point>>{p1, p2, p3, p4, args...}){};
        Polyhedron(std::vector<std::shared_ptr<Point>> vert);
        inline virtual unsigned int dim() const override {return 3;}
        inline virtual bool isSpace() const override {return false;}
        virtual float dist(const Point &obj) const override;
        virtual float dist(const Line &obj) const override;
        virtual float dist(const LinSeg &obj) const override;
        virtual float dist(const Plane &obj) const override;
        virtual float dist(const Polygon &obj) const override;
        virtual float dist(const Polyhedron &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const override;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polygon &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Polyhedron &obj) const override;
        float volume() const;
};

}

static std::ostream& operator<<(std::ostream &strm, const gmh::Point &p){
    if(p.vertices.size() == 0)
        return strm << "Point(" << p.pos.x << ", " << p.pos.y << ", " << p.pos.z << ")";
    strm << typeid(p).name() << '(';
    for(unsigned int i = 0; i < p.vertices.size() - 1; i++)
        strm << *p.vertices[i] << ", ";
    return strm << *p.vertices[p.vertices.size() - 1] << ")";
};
