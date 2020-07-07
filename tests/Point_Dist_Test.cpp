#include <gtest/gtest.h>
#include <Graphics/geometry.hpp>

struct PointDistTest: public ::testing::Test {
    Point p;
    virtual void SetUp() override {
        p = glm::vec3({0, 0, 0});
    }
};

TEST_F(PointDistTest, PointDistToPoint){
    Point p2({1, 1, 0});

    ASSERT_FLOAT_EQ(p2.dist(p), p.dist(p2));
    EXPECT_FLOAT_EQ(sqrt(2), p.dist(p2));

    p.pos = {1, 1, 0};
    ASSERT_FLOAT_EQ(p2.dist(p), p.dist(p2));
    EXPECT_FLOAT_EQ(0, p.dist(p2));
}

TEST_F(PointDistTest, PointDistToLine){
    Line lin(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0));

    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(1, p.dist(lin));

    p.pos.y = -999;
    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(1000, p.dist(lin));

    p.pos.y = 1001;
    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(1000, p.dist(lin));

    p.pos = {100, 1, 0};
    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(0, p.dist(lin));
}

TEST_F(PointDistTest, PointDistToLinSegNoClamp){
    LinSeg lin(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0));

    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(1, p.dist(lin));

    p.pos.y = -999;
    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(1000, p.dist(lin));

    p.pos.y = 1001;
    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(1000, p.dist(lin));

    p.pos = {0.5, 1, 0};
    ASSERT_FLOAT_EQ(lin.dist(p), p.dist(lin));
    EXPECT_FLOAT_EQ(0, p.dist(lin));
}

TEST_F(PointDistTest, PointDistToLinSegWithClamp){
    LinSeg lin1(glm::vec3(100, 1, 0), glm::vec3(101, 1, 0));
    LinSeg lin2(glm::vec3(-100, 1, 0), glm::vec3(-101, 1, 0));

    ASSERT_FLOAT_EQ(lin1.dist(p), lin2.dist(p));
    ASSERT_FLOAT_EQ(lin1.dist(p), p.dist(lin1));
    ASSERT_FLOAT_EQ(lin1.dist(p), p.dist(lin2));
    EXPECT_FLOAT_EQ(sqrt(10001), p.dist(lin1));

    p.pos.y = -99;
    ASSERT_FLOAT_EQ(lin1.dist(p), lin2.dist(p));
    ASSERT_FLOAT_EQ(lin1.dist(p), p.dist(lin1));
    ASSERT_FLOAT_EQ(lin1.dist(p), p.dist(lin2));
    EXPECT_FLOAT_EQ(100*sqrt(2), p.dist(lin1));

    p.pos.y = 101;
    ASSERT_FLOAT_EQ(lin1.dist(p), lin2.dist(p));
    ASSERT_FLOAT_EQ(lin1.dist(p), p.dist(lin1));
    ASSERT_FLOAT_EQ(lin1.dist(p), p.dist(lin2));
    EXPECT_FLOAT_EQ(100*sqrt(2), p.dist(lin1));
}

TEST_F(PointDistTest, PointDistToPlane){
    Plane plan(glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1));

    ASSERT_FLOAT_EQ(plan.dist(p), p.dist(plan));
    EXPECT_FLOAT_EQ(1, p.dist(plan));

    p.pos.y = 5;
    ASSERT_FLOAT_EQ(plan.dist(p), p.dist(plan));
    EXPECT_FLOAT_EQ(4, p.dist(plan));

    p.pos = {-4.5e5, 1, 8e2};
    ASSERT_FLOAT_EQ(plan.dist(p), p.dist(plan));
    EXPECT_FLOAT_EQ(0, p.dist(plan));
}

TEST_F(PointDistTest, PointDistToPolygonInPlane){
    Polygon poly(glm::vec3(1, 0, 0), glm::vec3(2, -1, 0), glm::vec3(2, 1, 0));

    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos.x = 5;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(3, p.dist(poly));

    p.pos = {2, 3, 0};
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(2, p.dist(poly));

    p.pos = {1, 1, 0};
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.5), p.dist(poly));

    p.pos = {1.5, 0, 0};
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(0, p.dist(poly));
}

TEST_F(PointDistTest, PointDistToPolygonNoClamp){
    Polygon poly(glm::vec3(-1, 0, 1), glm::vec3(1, -2, 1), glm::vec3(1, 2, 1));

    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos.z = 2;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos.x = -1;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos.x = 1;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));
}

TEST_F(PointDistTest, PointDistToPolygonWithClamp){
    Polygon poly(glm::vec3(-3, 0, 1), glm::vec3(-1, -2, 1), glm::vec3(-1, 2, 1));

    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(2), p.dist(poly));

    p.pos.z = 2;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(2), p.dist(poly));

    p.pos.y = 3;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(3), p.dist(poly));

    p.pos.x = -1;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(2), p.dist(poly));
}

TEST_F(PointDistTest, PointDistToPolyhedron){
    Polyhedron poly(glm::vec3(1, 0, 0), glm::vec3(2, -1, 0), glm::vec3(2, 1, 0), glm::vec3(1.5, 0, 1));

    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos = {1.5, 0, -1};
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos.z = 2;
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(1, p.dist(poly));

    p.pos = {1, 0, 1};
    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(sqrt(0.2), p.dist(poly));

    p.pos = {1.5, 0, 0.5};

    ASSERT_FLOAT_EQ(poly.dist(p), p.dist(poly));
    EXPECT_FLOAT_EQ(0, p.dist(poly));
}
