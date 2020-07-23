#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct LineInterTest: public ::testing::Test {
    Line obj;
    virtual void SetUp() override {
        obj = Line(glm::vec3(-5, -5, 0), glm::vec3(5, 5, 0));
    }

    template<typename T>
    inline void checkNoInter(const T& obj2){
        EXPECT_FALSE(obj2.intersect(obj)) << "Intersection: " << *obj2.intersect(obj);
        EXPECT_FALSE(obj.intersect(obj2)) << "Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }

    template<typename T1, typename T2>
    inline void checkYesInter(const T1& obj2, const T2&& inter){
        ASSERT_TRUE(obj2.intersect(obj));
        ASSERT_TRUE(obj.intersect(obj2));
        EXPECT_TRUE(obj.intersect(obj2)->equals(*obj2.intersect(obj))) << "obj.intersect(obj2): " << *obj.intersect(obj2) << std::endl << "obj2.intersect(obj): " << *obj2.intersect(obj);
        EXPECT_TRUE(obj.intersect(obj2)->equals(inter)) << "Expected Intersection: " << inter << std::endl << "Actual Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }
};

TEST_F(LineInterTest, LineInterWithLine){
    Line lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    checkNoInter(lin2);

    lin2 = Line(glm::vec3(-1, 1, 0), glm::vec3(1, -1, 1));
    checkNoInter(lin2);

    lin2 = Line(glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0));
    checkYesInter(lin2, Point({0, 0, 0}));

    lin2 = Line(glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0));
    checkYesInter(lin2, Line(glm::vec3(3, 3, 0), glm::vec3(4, 4, 0)));
}

TEST_F(LineInterTest, LineInterWithLinSeg){
    LinSeg lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    checkNoInter(lin2);

    lin2 = LinSeg(glm::vec3(-1, 1, 1), glm::vec3(1, -1, 2));
    checkNoInter(lin2);

    lin2 = LinSeg(glm::vec3(-1, 1, 0), glm::vec3(-2, 2, 0));
    checkNoInter(lin2);

    lin2 = LinSeg(glm::vec3(5, 6, 0), glm::vec3(6, 5, 0));
    checkYesInter(lin2, Point({5.5, 5.5, 0}));

    lin2 = LinSeg(glm::vec3(-2, -2, 0), glm::vec3(-5, -5, 0));
    checkYesInter(lin2, LinSeg(glm::vec3(-5, -5, 0), glm::vec3(-2, -2, 0)));
}

TEST_F(LineInterTest, LineInterWithPlane){
    Plane plan(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1));
    checkNoInter(plan);

    plan = Plane(glm::vec3(0, 1, 1), glm::vec3(1, 2, 1), glm::vec3(0, 1, 0));
    checkNoInter(plan);

    plan = Plane(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 2), glm::vec3(1, -1, 2));
    checkYesInter(plan, Point({0, 0, 0}));

    plan = Plane(glm::vec3(0, 0, 1), glm::vec3(1, 1, 2), glm::vec3(-1, -1, 2));
    checkYesInter(plan, Line(glm::vec3(100, 100, 0), glm::vec3(101, 101, 0)));
}

TEST_F(LineInterTest, LineInterWithPolygon){
    Polygon poly(glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, 1, 1));

    checkNoInter(poly);

    obj = Line(glm::vec3(2, 1, 1), glm::vec3(2, -1, 1));
    checkNoInter(poly);

    obj =Line(glm::vec3(2, 0, 1), glm::vec3(2, 0, 0));
    checkNoInter(poly);

    obj = Line(glm::vec3(-2, -2, 0), glm::vec3(2, 2, 2));
    checkYesInter(poly, Point({0, 0, 1}));

    obj = Line(glm::vec3(-2, -2, 1), glm::vec3(2, 2, 1));
    checkYesInter(poly, LinSeg(glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1)));

    obj = Line(glm::vec3(0, -2, 1), glm::vec3(0, 2, 1));
    checkYesInter(poly, LinSeg(glm::vec3(0, -1, 1), glm::vec3(0, 1, 1)));

    obj = Line(glm::vec3(2, 1, 1), glm::vec3(-5, 1, 1));
    checkYesInter(poly, LinSeg(glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1)));

    obj = Line(glm::vec3(2, 0, 1), glm::vec3(0, 2, 1));
    checkYesInter(poly, Point({1, 1, 1}));
}

TEST_F(LineInterTest, LineInterWithPolyhedron){
    Polyhedron poly(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1));

    checkYesInter(poly, LinSeg(glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0)));

    obj = Line(glm::vec3(-5, -5, -1), glm::vec3(5, 5, -1));
    checkNoInter(poly);

    obj = Line(glm::vec3(2, 0, 1), glm::vec3(2, 0, 0));
    checkNoInter(poly);

    obj = Line(glm::vec3(2, 1, 0), glm::vec3(2, -1, 0));
    checkNoInter(poly);

    obj = Line(glm::vec3(1, 2, 0), glm::vec3(1, -2, 0));
    checkYesInter(poly, LinSeg(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0)));

    obj = Line(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0.5));
    checkYesInter(poly, LinSeg(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)));

    obj = Line(glm::vec3(-2, 0, 0.5), glm::vec3(2, 0, 0.5));
    checkYesInter(poly, LinSeg(glm::vec3(-0.5, 0, 0.5), glm::vec3(0.5, 0, 0.5)));

    obj = Line(glm::vec3(-2, -2, 1), glm::vec3(-3, -3, 1));
    checkYesInter(poly, Point({0, 0, 1}));
}
