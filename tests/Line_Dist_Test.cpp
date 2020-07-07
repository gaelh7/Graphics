#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct LineDistTest: public ::testing::Test {
    Line lin;
    virtual void SetUp() override {
        lin = Line(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0));
    }
};

TEST_F(LineDistTest, LineDistToLine){
    Line lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = Line(glm::vec3(1, 0, 0), glm::vec3(1, 0, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(sqrt(0.5), lin.dist(lin2));

    lin2 = Line(glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));
}

TEST_F(LineDistTest, LineDistToLinSeg){
    LinSeg lin2(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = LinSeg(glm::vec3(0, 0, 1), glm::vec3(0, 0, 2));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = LinSeg(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));

    lin2 = LinSeg(glm::vec3(-1, 1, 1), glm::vec3(1, -1, 1));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = LinSeg(glm::vec3(-2, -2, 0), glm::vec3(-5, -5, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));
}

TEST_F(LineDistTest, LineDistToPlane){
    Plane plan(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(1, lin.dist(plan));

    plan = Plane(glm::vec3(0, 1, 1), glm::vec3(1, 2, 1), glm::vec3(0, 1, 0));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(sqrt(0.5), lin.dist(plan));

    plan = Plane(glm::vec3(0, 1, 1), glm::vec3(1, 2.1, 1), glm::vec3(0, 1, 0));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(0, lin.dist(plan));
}

TEST_F(LineDistTest, LineDistToPolygon){
    Polygon poly(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1), glm::vec3(0, 1, 1));

    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin.vertices[1]->pos = {2, 2, 0};
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = Line(glm::vec3(-1, 0.5, 0), glm::vec3(-1, 0.5, -8));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = Line(glm::vec3(-2, 0.8, 0), glm::vec3(0, 0.8, 2));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.5), lin.dist(poly));

    lin = Line(glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 3));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));
}

TEST_F(LineDistTest, LineDistToPolyhedron){
    Polyhedron poly(glm::vec3(-1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(0, 0, 1));

    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = Line(glm::vec3(-2, -1, 0), glm::vec3(-2, 1, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = Line(glm::vec3(-1, 0, 1), glm::vec3(-2, 0, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.5), lin.dist(poly));

    lin = Line(glm::vec3(-2, 0, -1.5), glm::vec3(2, 0, -1.5));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1.5, lin.dist(poly));

    lin = Line(glm::vec3(-2, 0, 1.5), glm::vec3(2, 0, 1.5));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0.5, lin.dist(poly));
}
