#pragma once

#include "Graphics/geometry.hpp"

#define ASSERT(x) if ((!x)) __debugbreak();
#ifdef GH_DEBUG
    #define GLCALL(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCALL(x) x
#endif

static void GLClearError(){
    while(glGetError());
}

static bool GLLogCall(const char* function, const char* file, int line){
    if(GLenum error = glGetError()){
        std::cout << "OpenGLError " << std::hex << error << ": " << file << "(" << std::dec << line << "): in " << function << std::endl;
        return false;
    }
    return true;
}

class Surface: public Polygon{
    private:
        unsigned int VAO, VBO, IBO;
        std::unique_ptr<double[]> VBO_DATA;
        std::unique_ptr<unsigned int[]> IBO_DATA;
    public:
        xt::xtensor_fixed<double, xt::xshape<3>> vel;
        xt::xtensor_fixed<double, xt::xshape<3>> acc;
        template<typename... Points>
        Surface(xt::xtensor_fixed<double, xt::xshape<3>> vel = {0, 0, 0}, xt::xtensor_fixed<double, xt::xshape<3>> acc = {0, 0, 0}, Points... args): Surface(vel, acc, std::vector<Point>{args...}){};
        Surface(xt::xtensor_fixed<double, xt::xshape<3>> vel, xt::xtensor_fixed<double, xt::xshape<3>> acc, std::vector<Point> vert);
        ~Surface();
        void update(double dt);
        // void rotate(xt::xtensor_fixed<double, xt::xshape<3>> axis, double theta, xt::xtensor_fixed<double, xt::xshape<3>> p);
        inline void bind() const{
            GLCALL(glBindVertexArray(VAO));
            GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
        }
        inline void render() const{
            GLCALL(glDrawElements(GL_TRIANGLES, 3*(vertices.size() - 2), GL_UNSIGNED_INT, nullptr));
        }
        void VBOSET(){
            for(unsigned int i = 0; i < 3*vertices.size(); i++)
                if(i%3!=2)
                    VBO_DATA[i] += 0.002;
            GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
            GLCALL(glBufferData(GL_ARRAY_BUFFER, 3*vertices.size()*sizeof(double), VBO_DATA.get(), GL_STATIC_DRAW));
        }
};

class Solid: public Polyhedron{};
