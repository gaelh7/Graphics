#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct PolyhedronInterTest: public ::testing::Test {
    Polyhedron obj;
    virtual void SetUp() override {
        obj = Polyhedron(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0));
    }

    template<typename T>
    inline void checkNoInter(const T& obj2){
        EXPECT_FALSE(obj2.intersect(obj)) << "Intersection: " << *obj2.intersect(obj);
        EXPECT_FALSE(obj.intersect(obj2)) << "Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }

    template<typename T1, typename T2>
    inline void checkYesInter(const T1& obj2, const T2&& inter){
        ASSERT_TRUE(obj.intersect(obj2));
        ASSERT_TRUE(obj2.intersect(obj));
        EXPECT_TRUE(obj.intersect(obj2)->equals(*obj2.intersect(obj))) << "obj.intersect(obj2): " << *obj.intersect(obj2) << std::endl << "obj2.intersect(obj): " << *obj2.intersect(obj);
        EXPECT_TRUE(obj.intersect(obj2)->equals(inter)) << "Expected Intersection: " << inter << std::endl << "Actual Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }
};

TEST_F(PolyhedronInterTest, PolyhedronInterWithPolyhedronOnSurface){
    Polyhedron poly2(glm::vec3(0, 0, -1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0));
    checkYesInter(poly2, Polygon(glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0)));

    poly2 = Polyhedron(glm::vec3(1, 1, 0), glm::vec3(3, 1, 0), glm::vec3(3, -1, 0), glm::vec3(1, -1, 0), glm::vec3(2, 0, -1));
    checkYesInter(poly2, LinSeg(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0)));

    poly2 = Polyhedron(glm::vec3(1, 3, 0), glm::vec3(3, 3, 0), glm::vec3(3, 1, 0), glm::vec3(1, 1, 0), glm::vec3(2, 2, -1));
    checkYesInter(poly2, Point({1, 1, 0}));

    poly2 = Polyhedron(glm::vec3(0.5, 2.5, 0.5), glm::vec3(2.5, 2.5, 0.5), glm::vec3(2.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.5, 1.5, 1.5));
    checkYesInter(poly2, Point({0.5, 0.5, 0.5}));

    poly2 = Polyhedron(glm::vec3(0, 0, 2), glm::vec3(-1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1));
    checkYesInter(poly2, Point({0, 0, 1}));

    poly2 = Polyhedron(glm::vec3(1.5, 0, 1.5), glm::vec3(0.5, 1, 0.5), glm::vec3(2.5, -1, 0.5), glm::vec3(0.5, -1, 0.5), glm::vec3(2.5, 1, 0.5));
    checkYesInter(poly2, LinSeg(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, -0.5, 0.5)));

    poly2 = Polyhedron(glm::vec3(1, 1, -1), glm::vec3(0, 2, 0), glm::vec3(2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(2, 2, 0));
    checkYesInter(poly2, Polygon(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0)));
}

TEST_F(PolyhedronInterTest, PolyhedronInterWithPolyhedronInside){
    Polyhedron poly2(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0));
    checkYesInter(poly2, Polyhedron(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0)));

    poly2 = Polyhedron(glm::vec3(0, 0, 2), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0));
    checkYesInter(poly2, Polyhedron(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0)));

    poly2 = Polyhedron(glm::vec3(0, 0, 2), glm::vec3(-2, 2, -1), glm::vec3(2, -2, -1), glm::vec3(-2, -2, -1), glm::vec3(2, 2, -1));
    checkYesInter(poly2, Polyhedron(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0)));

    poly2 = Polyhedron(glm::vec3(0, 0, 0.5), glm::vec3(-1, 1, -0.5), glm::vec3(1, -1, -0.5), glm::vec3(-1, -1, -0.5), glm::vec3(1, 1, -0.5));
    checkYesInter(poly2, Polyhedron(glm::vec3(0, 0, 0.5), glm::vec3(-0.5, 0.5, 0), glm::vec3(0.5, -0.5, 0), glm::vec3(-0.5, -0.5, 0), glm::vec3(0.5, 0.5, 0)));

    poly2 = Polyhedron(glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec3(2, -1, 0), glm::vec3(0, -1, 0), glm::vec3(2, 1, 0));
    checkYesInter(poly2, Polyhedron(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, -0.5, 0.5)));
}

TEST_F(PolyhedronInterTest, PolyhedronNoInterWithPolyhedron){
    Polyhedron poly2(glm::vec3(0, 0, -1), glm::vec3(-1, 1, -2), glm::vec3(1, -1, -2), glm::vec3(-1, -1, -2), glm::vec3(1, 1, -2));
    checkNoInter(poly2);

    poly2 = Polyhedron(glm::vec3(3, 0, 1), glm::vec3(2, 1, 0), glm::vec3(4, -1, 0), glm::vec3(2, -1, 0), glm::vec3(3, 1, 0));
    checkNoInter(poly2);

    poly2 = Polyhedron(glm::vec3(0, 0, -2), glm::vec3(-1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1), glm::vec3(1, 1, -1));
    checkNoInter(poly2);
}
