#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct LinSegDistTest: public ::testing::Test {
    gmh::LinSeg lin;
    virtual void SetUp() override {
        lin = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
    }
};

TEST_F(LinSegDistTest, LinSegDistToLinSeg){
    gmh::LinSeg lin2(glm::vec3(0.5, 1, 0), glm::vec3(1.5, 1, 0));

    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(0.5, 1, 0), glm::vec3(0.5, 2, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(2, 1, 0), glm::vec3(3, 2, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(sqrt(2), lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(-1, 1, 0), glm::vec3(-1, -1, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(1, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(-1, 0, 0), glm::vec3(2, 0, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(0.5, 0, 0), glm::vec3(2, 0, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));

    lin2 = gmh::LinSeg(glm::vec3(-1, -1.5, 0), glm::vec3(2, 1.5, 0));
    ASSERT_FLOAT_EQ(lin2.dist(lin), lin.dist(lin2));
    EXPECT_FLOAT_EQ(0, lin.dist(lin2));
}

TEST_F(LinSegDistTest, LinSegDistToPlane){
    gmh::Plane plan(glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(0, 1, 1));

    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(1, lin.dist(plan));

    lin = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(1, lin.dist(plan));

    lin = gmh::LinSeg(glm::vec3(0, 0, 2), glm::vec3(1, 1, 3));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(1, lin.dist(plan));

    lin = gmh::LinSeg(glm::vec3(0, 0, 0), glm::vec3(0, 0, 2));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(0, lin.dist(plan));

    lin = gmh::LinSeg(glm::vec3(0, 0, 1), glm::vec3(0, 0, 2));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(0, lin.dist(plan));

    lin = gmh::LinSeg(glm::vec3(20, -40, 1), glm::vec3(-11, 9, 1));
    ASSERT_FLOAT_EQ(plan.dist(lin), lin.dist(plan));
    EXPECT_FLOAT_EQ(0, lin.dist(plan));
}

TEST_F(LinSegDistTest, LinSegDistToPolygon){
    gmh::Polygon poly(glm::vec3(-1, -1, 0), glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(1, 1, 0));

    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(2, 0, 0), glm::vec3(2, 1, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(2, 0, 0), glm::vec3(0, 2, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(4, 0, 0), glm::vec3(0, 4, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(2), lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(2, -1, 1), glm::vec3(2, 1, 1));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(2), lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(0, 0, 1), glm::vec3(2, 1, 1));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(2, 1, -1), glm::vec3(2, 1, 1));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));
}

TEST_F(LinSegDistTest, LinSegDistToPolyhedron){
    gmh::Polyhedron poly(glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, -1, 0), glm::vec3(0.5, 0, 1));

    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(1, 2, 0), glm::vec3(1, -2, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(0, 2, 0), glm::vec3(0, -2, 0));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(0, 2, 1), glm::vec3(0, -2, 1));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.2), lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(0.5, 2, -1), glm::vec3(0.5, -2, -1));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(0.5, 2, 0.5), glm::vec3(0.5, -2, 0.5));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(0, lin.dist(poly));

    lin = gmh::LinSeg(glm::vec3(0.5, 2, 2), glm::vec3(0.5, -2, 2));
    ASSERT_FLOAT_EQ(poly.dist(lin), lin.dist(poly));
    EXPECT_FLOAT_EQ(1, lin.dist(poly));
}
