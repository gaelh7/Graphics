#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct PlaneDistTest: public ::testing::Test {
    gmh::Plane obj;

    virtual void SetUp() override {
        obj = gmh::Plane(glm::vec3(0, -8, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
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

TEST_F(PlaneDistTest, PlaneDistToPlane){
    gmh::Plane plan1(glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 1));
    checkNoInter(plan1);

    plan1 = gmh::Plane(glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 0));
    checkYesInter(plan1, gmh::Line(glm::vec3(0, -2, 0), glm::vec3(0, 2, 0)));

    plan1 = gmh::Plane(glm::vec3(-1, -1, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, -1));
    checkYesInter(plan1, gmh::Line(glm::vec3(0, 0, -2), glm::vec3(0, 0, 2)));

    plan1 = gmh::Plane(glm::vec3(0, 8, 9), glm::vec3(0, 21, 12), glm::vec3(0, -18, 4));
    checkYesInter(plan1, gmh::Plane(glm::vec3(0, 1, 2), glm::vec3(0, 4, 4), glm::vec3(0, 5, 6)));
}

TEST_F(PlaneDistTest, PlaneDistToPolygon){
    gmh::Polygon poly(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(1, 2, 3));
    checkNoInter(poly);

    poly = gmh::Polygon(glm::vec3(-21, 2, 2), glm::vec3(-21, 3, 2), glm::vec3(-21, 2, 3));
    checkNoInter(poly);

    poly = gmh::Polygon(glm::vec3(0, 2, 2), glm::vec3(0, 3, 2), glm::vec3(0, 2, 3));
    checkYesInter(poly, gmh::Polygon(glm::vec3(0, 2, 2), glm::vec3(0, 3, 2), glm::vec3(0, 2, 3)));

    poly = gmh::Polygon(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(2, 2.5, 2));
    checkNoInter(poly);

    poly = gmh::Polygon(glm::vec3(2, 2, 2), glm::vec3(2, 3, 2), glm::vec3(1, 2.5, 2));
    checkNoInter(poly);

    poly = gmh::Polygon(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(-1, 2.5, 2));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 2.25, 2), glm::vec3(0, 2.75, 2)));

    poly = gmh::Polygon(glm::vec3(0, 2, 2), glm::vec3(0, 3, 2), glm::vec3(-1, 2.5, 2));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 2, 2), glm::vec3(0, 3, 2)));

    poly = gmh::Polygon(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(0, 2.5, 2));
    checkYesInter(poly, gmh::Point({0, 2.5, 2}));
}

TEST_F(PlaneDistTest, PlaneDistToPolyhedron){
    gmh::Polyhedron poly(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(2, 0, 0));
    checkNoInter(poly);

    poly = gmh::Polyhedron(glm::vec3(2, 1, 1), glm::vec3(2, -1, 1), glm::vec3(2, 1, -1), glm::vec3(2, -1, -1), glm::vec3(1, 0, 0));
    checkNoInter(poly);

    poly = gmh::Polyhedron(glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(2, 1, -1), glm::vec3(2, -1, -1), glm::vec3(3, 0, 0));
    checkNoInter(poly);

    poly = gmh::Polyhedron(glm::vec3(0, 1, 1), glm::vec3(0, -1, 1), glm::vec3(0, 1, -1), glm::vec3(0, -1, -1), glm::vec3(1, 0, 0));
    checkYesInter(poly, gmh::Polygon(glm::vec3(0, 1, 1), glm::vec3(0, -1, 1), glm::vec3(0, 1, -1), glm::vec3(0, -1, -1)));

    poly = gmh::Polyhedron(glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), glm::vec3(0, 0, 0));
    checkYesInter(poly, gmh::Point({0, 0, 0}));

    poly = gmh::Polyhedron(glm::vec3(-1, 1, 1), glm::vec3(-1, -1, 1), glm::vec3(-1, 1, -1), glm::vec3(-1, -1, -1), glm::vec3(1, 0, 0));
    checkYesInter(poly, gmh::Polygon(glm::vec3(0, 0.5, 0.5), glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, -0.5, -0.5)));

    poly = gmh::Polyhedron(glm::vec3(0, 1, 1), glm::vec3(0, -1, 1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(2, 0, 0));
    checkYesInter(poly, gmh::LinSeg(glm::vec3(0, 1, 1), glm::vec3(0, -1, 1)));
}
