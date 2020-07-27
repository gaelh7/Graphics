#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct LineInterTest: public ::testing::Test {
    gmh::Line obj;
    virtual void SetUp() override {
        obj = gmh::Line(glm::vec3(-5, -5, 0), glm::vec3(5, 5, 0));
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
    gmh::Line lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    checkNoInter(lin2);

    lin2 = gmh::Line(glm::vec3(-1, 1, 0), glm::vec3(1, -1, 1));
    checkNoInter(lin2);

    lin2 = gmh::Line(glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0));
    checkYesInter(lin2, gmh::Point({0, 0, 0}));

    lin2 = gmh::Line(glm::vec3(0, 0, 0), glm::vec3(0.1, 0.1, 0));
    checkYesInter(lin2, gmh::Line(glm::vec3(3, 3, 0), glm::vec3(4, 4, 0)));
}

TEST_F(LineInterTest, LineInterWithLinSeg){
    gmh::LinSeg lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    checkNoInter(lin2);

    lin2 = gmh::LinSeg(glm::vec3(-1, 1, 1), glm::vec3(1, -1, 2));
    checkNoInter(lin2);

    lin2 = gmh::LinSeg(glm::vec3(-1, 1, 0), glm::vec3(-2, 2, 0));
    checkNoInter(lin2);

    lin2 = gmh::LinSeg(glm::vec3(5, 6, 0), glm::vec3(6, 5, 0));
    checkYesInter(lin2, gmh::Point({5.5, 5.5, 0}));

    lin2 = gmh::LinSeg(glm::vec3(-2, -2, 0), glm::vec3(-5, -5, 0));
    checkYesInter(lin2, gmh::LinSeg(glm::vec3(-5, -5, 0), glm::vec3(-2, -2, 0)));
}

TEST_F(LineInterTest, LineInterWithPlane){
    gmh::Plane plan(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1));
    checkNoInter(plan);

    plan = gmh::Plane(glm::vec3(0, 1, 1), glm::vec3(1, 2, 1), glm::vec3(0, 1, 0));
    checkNoInter(plan);

    plan = gmh::Plane(glm::vec3(0, 0, 1), glm::vec3(-1, 1, 2), glm::vec3(1, -1, 2));
    checkYesInter(plan, gmh::Point({0, 0, 0}));

    plan = gmh::Plane(glm::vec3(0, 0, 1), glm::vec3(1, 1, 2), glm::vec3(-1, -1, 2));
    checkYesInter(plan, gmh::Line(glm::vec3(100, 100, 0), glm::vec3(101, 101, 0)));
}

TEST_F(LineInterTest, LineInterWithPolygon){
    gmh::Polygon poly(glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, 1, 1));

    checkNoInter(poly);

    obj = gmh::Line(glm::vec3(2, 1, 1), glm::vec3(2, -1, 1));
    checkNoInter(poly);

    obj =gmh::Line(glm::vec3(2, 0, 1), glm::vec3(2, 0, 0));
    checkNoInter(poly);

    obj = gmh::Line(glm::vec3(-2, -2, 0), glm::vec3(2, 2, 2));
    checkYesInter(poly, gmh::Point({0, 0, 1}));

    obj = gmh::Line(glm::vec3(-2, -2, 1), glm::vec3(2, 2, 1));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(-1, -1, 1), glm::vec3(1, 1, 1)));

    obj = gmh::Line(glm::vec3(0, -2, 1), glm::vec3(0, 2, 1));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, -1, 1), glm::vec3(0, 1, 1)));

    obj = gmh::Line(glm::vec3(2, 1, 1), glm::vec3(-5, 1, 1));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(1, 1, 1), glm::vec3(-1, 1, 1)));

    obj = gmh::Line(glm::vec3(2, 0, 1), glm::vec3(0, 2, 1));
    checkYesInter(poly, gmh::Point({1, 1, 1}));
}

TEST_F(LineInterTest, LineInterWithPolyhedron){
    gmh::Polyhedron poly(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(-1, -1, 0), glm::vec3(0, 0, 1));

    checkYesInter(poly, gmh::LinSeg(glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0)));

    obj = gmh::Line(glm::vec3(-5, -5, -1), glm::vec3(5, 5, -1));
    checkNoInter(poly);

    obj = gmh::Line(glm::vec3(2, 0, 1), glm::vec3(2, 0, 0));
    checkNoInter(poly);

    obj = gmh::Line(glm::vec3(2, 1, 0), glm::vec3(2, -1, 0));
    checkNoInter(poly);

    obj = gmh::Line(glm::vec3(1, 2, 0), glm::vec3(1, -2, 0));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(1, 1, 0), glm::vec3(1, -1, 0)));

    obj = gmh::Line(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0.5));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0)));

    obj = gmh::Line(glm::vec3(-2, 0, 0.5), glm::vec3(2, 0, 0.5));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(-0.5, 0, 0.5), glm::vec3(0.5, 0, 0.5)));

    obj = gmh::Line(glm::vec3(-2, -2, 1), glm::vec3(-3, -3, 1));
    checkYesInter(poly, gmh::Point({0, 0, 1}));
}
