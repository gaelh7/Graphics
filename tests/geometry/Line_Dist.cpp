#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct LineDistTest: public ::testing::Test {
    gmh::Line lin;
    virtual void SetUp() override {
        lin = gmh::Line(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    }
};

TEST_F(LineDistTest, LineDistToLine){
    gmh::Line lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::Line(glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(sqrt(0.5f), lin.dist(lin2));

    lin2 = gmh::Line(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));
}

TEST_F(LineDistTest, LineDistToLinSeg){
    gmh::LinSeg lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(0, 0, 1), glm::vec3(0, 0, 2));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(-1, 1, 1), glm::vec3(1, -1, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(-2, -2, 0), glm::vec3(-5, -5, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));
}

TEST_F(LineDistTest, LineDistToPlane){
    gmh::Plane plan(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(1, lin.dist(plan));

    plan = gmh::Plane(glm::vec3(0, 1, 1), glm::vec3(1, 2, 1), glm::vec3(0, 1, 0));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(sqrt(0.5f), lin.dist(plan));

    plan = gmh::Plane(glm::vec3(0, 1, 1), glm::vec3(1, 2.1, 1), glm::vec3(0, 1, 0));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(0, lin.dist(plan));
}

TEST_F(LineDistTest, LineDistToPolygon){
    gmh::Polygon poly(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1), glm::vec3(0, 1, 1));

    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin.vertices[1]->pos = {2, 2, 0};
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = gmh::Line(glm::vec3(-1, 0.5, 0), glm::vec3(-1, 0.5, -8));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = gmh::Line(glm::vec3(-2, 0.8, 0), glm::vec3(0, 0.8, 2));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.5f), lin.dist(poly));

    lin = gmh::Line(glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 3));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));
}

TEST_F(LineDistTest, LineDistToPolyhedron){
    gmh::Polyhedron poly(glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(0, 0, 1));

    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::Line(glm::vec3(-2, -1, 0), glm::vec3(-2, 1, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = gmh::Line(glm::vec3(-1, 0, 1), glm::vec3(-2, 0, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.5f), lin.dist(poly));

    lin = gmh::Line(glm::vec3(-2, 0, -1.5), glm::vec3(2, 0, -1.5));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1.5, lin.dist(poly));

    lin = gmh::Line(glm::vec3(-2, 0, 1.5), glm::vec3(2, 0, 1.5));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0.5, lin.dist(poly));
}
