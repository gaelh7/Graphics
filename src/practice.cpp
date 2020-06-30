#include <iostream>
#include <string>
#include <chrono>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/geometry.hpp"

#ifndef NDEBUG
    #define PRINT std::cout << "Debug Mode" << std::endl
#else
    #define PRINT std::cout << "Release Mode" << std::endl
#endif

using namespace std;

template <typename T>
std::ostream& operator<<(std::ostream &strm, const vector<std::shared_ptr<T>> &v){
    strm << "{";
    for(unsigned int i = 0; i < v.size() - 1; i++) strm << *v[i] << ", \n";
    return strm << *v[v.size() - 1] << "}";
}

int main(int argc, char* argv[])
{
    vector<int> v{1,2,3,4,5,6};
    PRINT;
    std::shared_ptr<Point> t(new Point({0,0,0}));
    // Point t({0,0,0});
    std::shared_ptr<Point> t1(new Point({1,1,0}));
    LinSeg t2 = LinSeg(t1, t);
    {
        LinSeg l(t, t1);
        cout << t.use_count() << endl;
    }
    cout << *t << endl;
    cout << *t2.vertices[0] << ", " << *t2.vertices[1] << endl;
    t->pos += glm::vec3(2,2,0);
    cout << *t2.vertices[0] << ", " << *t2.vertices[1] << endl;
    cout << "--------------------" << endl;
    LinSeg a(glm::vec3(1,1,1), glm::vec3(2,2,2));
    cout << a << endl;
    Point p1({-1,1,0});
    Point p2({1,1,0});
    Point p3({-1,-1,0});
    Point p4({1,-1,0});
    Point p5({0,0,1});
    Point p6({-1,1,2});
    Point p7({1,1,2});
    Point p8({-1,-1,2});
    Point p9({1,-1,2});
    Line l1(p1, p2);
    Plane l2(p7, p8, p9);
    LinSeg l3(Point({ 1.,  1.,  0.}), Point({-1.,  1.,  0.}));
    LinSeg l4(Point({-1.,  -1.,  0.}), Point({ 1.,  -1.,  0.}));


    Polygon x(p6,p7,p8,p9);

    Polyhedron poly = *reinterpret_cast<Polyhedron*>(&x);

    std::cout << poly << std::endl;

    auto ptr = std::make_shared<int>(1);
    auto ptr1 = ptr;
    auto ptr2 = ptr;
    cout << ptr2.use_count() << endl;

    Polyhedron y(p1,p2,p3,p4,p5);
    std::cout << y << std::endl;
    Polyhedron poly1 = Polyhedron(Point(glm::vec3(0,0,0)), Point(glm::vec3(1,0,0)), Point(glm::vec3(0,1,0)), Point(glm::vec3(0,0,1)), Point(glm::vec3(1,1,0)), Point(glm::vec3(1,0,1)), Point(glm::vec3(0,1,1)), Point(glm::vec3(1,1,1)));
    Polyhedron poly2 = Polyhedron(Point(glm::vec3(0.5, 1, 0.5)), Point(glm::vec3(1.5,1,0.5)), Point(glm::vec3(0.5,2,0.5)), Point(glm::vec3(0.5,1,1.5)), Point(glm::vec3(1.5,2,0.5)), Point(glm::vec3(1.5,1,1.5)), Point(glm::vec3(0.5,2,1.5)), Point(glm::vec3(1.5,2,1.5)));

    cout << poly1.intersect(poly2) << endl;

    cout << "Point: " << sizeof(Point) << " bytes" << endl;
    cout << "Line: " << sizeof(Line) << " bytes" << endl;
    cout << "LinSeg: " << sizeof(LinSeg) << " bytes" << endl;
    cout << "Plane: " << sizeof(Plane) << " bytes" << endl;
    cout << "Polygon: " << sizeof(Polygon) << " bytes" << endl;
    cout << "Polyhedron: " << sizeof(Polyhedron) << " bytes" << endl;

    cout << *l2.intersect(x) << endl;

    cout << y.faces << endl;

    cout << "test: " << *poly1.intersect(poly2) << "\n";

    auto start = chrono::high_resolution_clock::now();

    for(unsigned int i = 0; i < 1; i++)
        y.dist(x);

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

    cout << "Time taken: " << duration.count()/1000000000. << endl;

    // cout << y.volume() << endl;

    // cout << *x.intersect(l1) << endl;

    // cout << y.contains(l2) << endl;

    std::cout << "Press return to continue...";
    cin.get();

    return 0;
}
