#include "geometry.hpp"

class Surface: public Polygon{
    private:
        std::unique_ptr<double[]> VBO_DATA;
        unsigned int VBO;
        std::unique_ptr<unsigned int[]> IBO_DATA;
        unsigned int IBO;
    public:
        template<typename... Points>
        Surface(Points... args): Polygon({args...}){};
        Surface(std::vector<Point> vert);
        void render() const;
};