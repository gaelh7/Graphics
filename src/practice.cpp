#include <iostream>
#include <string>
#include <chrono>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/geometry.hpp"
#include "Graphics/hash.hpp"
#include "Graphics/log.hpp"

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
    std::atexit([](){
        std::cout << "Allocations: " << allocations << std::endl;
        std::cout << "Deletions: " << deletions << std::endl;
        std::cout << bytes_allocated << " Bytes allocated" << std::endl;
    });

    cout << "Point: " << sizeof(gmh::Point) << " bytes" << endl;
    cout << "Line: " << sizeof(gmh::Line) << " bytes" << endl;
    cout << "LinSeg: " << sizeof(gmh::LinSeg) << " bytes" << endl;
    cout << "Plane: " << sizeof(gmh::Plane) << " bytes" << endl;
    cout << "Polygon: " << sizeof(gmh::Polygon) << " bytes" << endl;
    cout << "Polyhedron: " << sizeof(gmh::Polyhedron) << " bytes" << endl;

    gmh::Line lin(glm::vec3(0, 0, 0), glm::vec3(-1, 2, 3));

    gmh::Point p({2, 1, 0});

    std::cout <<  lin.project(p) << std::endl;

    gmh::Line lin2(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    auto start = chrono::high_resolution_clock::now();

    cout << (gmh::operator!=(lin2, lin) ? "True\n":"False\n");

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);

    cout << "Time taken: " << duration.count()/1000000000. << endl;

    std::cout << "Press return to continue...";
    cin.get();

    return 0;
}
