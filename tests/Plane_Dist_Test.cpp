#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct PlaneDistTest: public ::testing::Test {
    Plane plan;

    virtual void SetUp() override {
        plan = Plane(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
    }
};

TEST_F(PlaneDistTest, PlaneDistToPlane){
    Plane plan1(glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 1));
    ASSERT_FLOAT_EQ(plan1.dist(plan), plan.dist(plan1));
    EXPECT_FLOAT_EQ(1, plan.dist(plan1));

    plan1 = Plane(glm::vec3(-1, 0, 0), glm::vec3(-1, 1, 0), glm::vec3(-1, 0, 1));
    ASSERT_FLOAT_EQ(plan1.dist(plan), plan.dist(plan1));
    EXPECT_FLOAT_EQ(1, plan.dist(plan1));

    plan1 = Plane(glm::vec3(-2, 0, 0), glm::vec3(-1, 1, 0), glm::vec3(-1, 0, 1));
    ASSERT_FLOAT_EQ(plan1.dist(plan), plan.dist(plan1));
    EXPECT_FLOAT_EQ(0, plan.dist(plan1));

    plan1 = Plane(glm::vec3(1, 0, 0), glm::vec3(-1, 1, 0), glm::vec3(1, 0, 1));
    ASSERT_FLOAT_EQ(plan1.dist(plan), plan.dist(plan1));
    EXPECT_FLOAT_EQ(0, plan.dist(plan1));
}

TEST_F(PlaneDistTest, PlaneDistToPolygon){
    Polygon poly(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(1, 2, 3));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));

    poly = Polygon(glm::vec3(-21, 2, 2), glm::vec3(-21, 3, 2), glm::vec3(-21, 2, 3));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(21, plan.dist(poly));

    poly = Polygon(glm::vec3(0, 2, 2), glm::vec3(0, 3, 2), glm::vec3(0, 2, 3));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(0, plan.dist(poly));

    poly = Polygon(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(2, 2.5, 2));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));

    poly = Polygon(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(-2, 2.5, 2));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(0, plan.dist(poly));

    poly = Polygon(glm::vec3(2, 2, 2), glm::vec3(2, 3, 2), glm::vec3(1, 2.5, 2));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));
}

TEST_F(PlaneDistTest, PlaneDistToPolyhedron){
    Polyhedron poly(glm::vec3(1, 2, 2), glm::vec3(1, 3, 2), glm::vec3(1, 2, 3), glm::vec3(2, 2.5, 2.5));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));

    poly = Polyhedron(glm::vec3(-1, 2, 2), glm::vec3(-1, 3, 2), glm::vec3(-1, 2, 3), glm::vec3(-2, 2.5, 2.5));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));

    poly = Polyhedron(glm::vec3(-1, 2, 2), glm::vec3(-1, 3, 2), glm::vec3(-1, 2, 3), glm::vec3(-2, 2.5, 2.5));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));

    poly = Polyhedron(glm::vec3(0, 2, 2), glm::vec3(0, 3, 2), glm::vec3(0, 2, 3), glm::vec3(-2, 2.5, 2.5));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(0, plan.dist(poly));

    poly = Polyhedron(glm::vec3(2, 2, 2), glm::vec3(2, 3, 2), glm::vec3(2, 2, 3), glm::vec3(1, 2.5, 2.5));
    ASSERT_FLOAT_EQ(poly.dist(plan), plan.dist(poly));
    EXPECT_FLOAT_EQ(1, plan.dist(poly));
}
