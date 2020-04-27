#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include "xtensor/xio.hpp"
#include "xtensor/xfixed.hpp"

#define PI 3.141592653589793238463

static xt::xtensor_fixed<double, xt::xshape<3>> cross(xt::xtensor_fixed<double, xt::xshape<3>> arr1, xt::xtensor_fixed<double, xt::xshape<3>> arr2){
    double i = arr1[1]*arr2[2] - arr1[2]*arr2[1];
    double j = arr1[2]*arr2[0] - arr1[0]*arr2[2];
    double k = arr1[0]*arr2[1] - arr1[1]*arr2[0];
    return {i, j, k};
};

template <typename T, class iter>
std::vector<std::vector<T>> combinations(iter first, iter last, int k){
    std::vector<std::vector<T>> out;
    if(k == 1){
        for(iter i = first; i != last; i++) out.push_back(std::vector<T>{*i});
        return out;
    }
    for(iter i = first; i != last; i++){
        std::vector<T> next;
        for(std::vector<T> end: combinations<T>(i+1, last, k-1)){
            next = {*i};
            next.insert(next.end(), end.begin(), end.end());
            out.push_back(next);
        }
    }
    return out;
};

class Point {
    public:
        std::vector<Point> vertices;
        xt::xtensor_fixed<double, xt::xshape<3>> pos;
        Point(){};
        Point(xt::xtensor_fixed<double, xt::xshape<3>> pos);
        virtual unsigned int dim() const {return 0;};
        virtual bool isSpace() const {return true;};
        virtual double dist(const Point &obj) const;
        virtual std::unique_ptr<Point> intersect (const Point &obj) const;
        xt::xtensor_fixed<double, xt::xshape<3>> direction(const Point &obj) const;
        bool contains(const Point &obj) const;
        bool equals(const Point &obj) const;
};

class Line: public Point {
    protected:
        Point p1;
        Point p2;
    public:
        Line(){};
        Line(Point p1, Point p2);
        Line(std::vector<Point> vert);
        unsigned int dim() const override {return 1;};
        double dist(const Point &obj) const override;
        virtual double dist(const Line &obj) const;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const;
        xt::xtensor_fixed<double, xt::xshape<3>> dirVec() const;
        std::unique_ptr<Point> project(const Point &obj) const;
        double angle(const Line &lobj, xt::xtensor_fixed<double, xt::xshape<3>>* axisptr);
};

class LinSeg: public Line {
    public:
        LinSeg(){};
        LinSeg(Point p1, Point p2);
        LinSeg(std::vector<Point> vert);
        bool isSpace() const override {return false;};
        double dist(const Point &obj) const override;
        double dist(const Line &obj) const override;
        double dist(const LinSeg &obj) const;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        std::unique_ptr<Point> intersect(const Line &obj) const override;
        std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        double length() const;
};

class Plane: public Point {
    protected:
        Point p1;
        Point p2;
        Point p3;
    public:
        Plane(){};
        Plane(Point p1, Point p2, Point p3);
        Plane(std::vector<Point> vert);
        unsigned int dim() const override {return 2;};
        xt::xtensor_fixed<double, xt::xshape<3>> normVec() const;
        std::unique_ptr<Point> project(const Point &obj) const;
        template<typename T>
        std::unique_ptr<T> project(const T &obj) const {
            std::vector<Point> v;
            for(Point p: obj.vertices) v.push_back(*project(p));
            return std::unique_ptr<T>(new T(v));
        };;
        double sign_dist(const Point &obj) const;
        double dist(const Point &obj) const override;
        virtual double dist(const Line &obj) const;
        virtual double dist(const LinSeg &obj) const;
        virtual double dist(const Plane &obj) const;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        virtual std::unique_ptr<Point> intersect(const Line &obj) const;
        virtual std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        virtual std::unique_ptr<Point> intersect(const Plane &obj) const;
};

class Polygon: public Plane {
    public:
        std::vector<LinSeg> edges;
        Polygon(){};
        template <typename... Points>
        Polygon(Point p1, Point p2, Point p3, Points... args): Polygon(std::vector<Point>{p1, p2, p3, args...}){};
        Polygon(std::vector<Point> vert);
        bool isSpace() const override {return false;};
        double dist(const Point &obj) const override;
        double dist(const Line &obj) const override;
        double dist(const LinSeg &obj) const override;
        double dist(const Plane &obj) const override;
        double dist(const Polygon &obj) const;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        std::unique_ptr<Point> intersect(const Line &obj) const override;
        std::unique_ptr<Point> intersect(const LinSeg &obj) const override;
        std::unique_ptr<Point> intersect(const Plane &obj) const override;
        std::unique_ptr<Point> intersect(const Polygon &obj) const;
        double area() const;
};

class Polyhedron: public Point {
    public:
        std::vector<LinSeg> edges;
        std::vector<Polygon> faces;
        Polyhedron(){};
        template <typename... Points>
        Polyhedron(Point p1, Point p2, Point p3, Point p4, Points... args): Polyhedron(std::vector<Point>{p1, p2, p3, p4, args...}) {};
        Polyhedron(std::vector<Point> vert);
        unsigned int dim() const override {return 3;};
        bool isSpace() const override {return false;};
        double dist(const Point &obj) const override;
        double dist(const Line &obj) const;
        double dist(const LinSeg &obj) const;
        double dist(const Plane &obj) const;
        double dist(const Polygon &obj) const;
        double dist(const Polyhedron &obj) const;
        std::unique_ptr<Point> intersect(const Point &obj) const override;
        // std::unique_ptr<Point> intersect(const Line &obj) const;
        // std::unique_ptr<Point> intersect(const LinSeg &obj) const;
        // std::unique_ptr<Point> intersect(const Plane &obj) const;
        // std::unique_ptr<Point> intersect(const Polygon &obj) const;
        // std::unique_ptr<Point> intersect(const Polyhedron &obj) const;
        double volume() const;
};

static std::ostream& operator<<(std::ostream &strm, const Point &p){
    if(p.vertices.size() == 1)
        return strm << "Point(" << p.pos << ")";
    strm << typeid(p).name() << '(';
    for(unsigned int i = 0; i < p.vertices.size() - 1; i++)
        strm << p.vertices[i] << ", ";
    return strm << p.vertices[p.vertices.size() - 1] << ")";
};
