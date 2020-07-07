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
    PRINT;

    cout << "Point: " << sizeof(Point) << " bytes" << endl;
    cout << "Line: " << sizeof(Line) << " bytes" << endl;
    cout << "LinSeg: " << sizeof(LinSeg) << " bytes" << endl;
    cout << "Plane: " << sizeof(Plane) << " bytes" << endl;
    cout << "Polygon: " << sizeof(Polygon) << " bytes" << endl;
    cout << "Polyhedron: " << sizeof(Polyhedron) << " bytes" << endl;

    Line lin(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));

    Line lin2(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    auto start = chrono::high_resolution_clock::now();

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

    cout << "Time taken: " << duration.count()/1000000000. << endl;

    std::cout << "Press return to continue...";
    cin.get();

    return 0;
}
