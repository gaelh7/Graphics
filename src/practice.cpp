#include <iostream>
#include <string>
#include <chrono>
#include "xtensor/xarray.hpp"
#include "geometry.hpp"

using namespace std;

template <typename T>
std::ostream& operator<<(std::ostream &strm, const vector<T> &v){
    strm << "{";
    for(unsigned int i = 0; i < v.size() - 1; i++) strm << v[i] << ", \n";
    return strm << v[v.size() - 1] << "}";
}

int main(int argc, char* argv[])
{
    vector<int> v{1,2,3,4,5,6};

    // cout << xt::norm({1,1,1}) << endl;

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
    Polyhedron y(p1,p2,p3,p4,p5);

    cout << y.faces << endl;

    cout << y.dist(x) << "\n";

    auto start = chrono::high_resolution_clock::now();

    for(unsigned int i = 0; i < 1; i++)
        y.dist(x);

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

    cout << "Time taken: " << duration.count()/1000000000. << endl;

    // cout << y.volume() << endl;

    // cout << *x.intersect(l1) << endl;

    // cout << y.contains(l2) << endl;

    return 0;
}