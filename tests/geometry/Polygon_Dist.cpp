#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"

struct PolygonDistTest: public ::testing::Test {
    gmh::Polygon poly;
    virtual void SetUp() override {
        poly = gmh::Polygon(glm::vec3(2, 2, 0), glm::vec3(-2, 2, 0), glm::vec3(2, -2, 0), glm::vec3(-2, -2, 0));
    }
};

TEST_F(PolygonDistTest, PolygonDistToPolygon){
    gmh::Polygon poly2(glm::vec3(0, 0, 0), glm::vec3(1, 0, 1), glm::vec3(-1, 0, 1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(0, poly.dist(poly2));

    poly2 = gmh::Polygon(glm::vec3(0, 1, 1), glm::vec3(1, 0, 1), glm::vec3(-1, 0, 1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(1, poly.dist(poly2));

    poly2 = gmh::Polygon(glm::vec3(0, 1, -1), glm::vec3(1, 0, -1), glm::vec3(-1, 0, -1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(1, poly.dist(poly2));

    poly2 = gmh::Polygon(glm::vec3(5, 0, 0), glm::vec3(0, 5, 0), glm::vec3(5, 5, 0));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(sqrt(0.5), poly.dist(poly2));

    poly2 = gmh::Polygon(glm::vec3(5, 0, 1), glm::vec3(0, 5, 1), glm::vec3(5, 5, 1));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(sqrt(1.5), poly.dist(poly2));

    poly2 = gmh::Polygon(glm::vec3(3, 3, 0), glm::vec3(3, -3, 0), glm::vec3(3, 0, 2));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(1, poly.dist(poly2));

    poly2 = gmh::Polygon(glm::vec3(3, 3, 1), glm::vec3(3, -3, 1), glm::vec3(3, 0, 2));
    ASSERT_FLOAT_EQ(poly2.dist(poly), poly.dist(poly2));
    EXPECT_FLOAT_EQ(sqrt(2), poly.dist(poly2));
}

TEST_F(PolygonDistTest, PolygonDistToPolyhedron){
    gmh::Polyhedron polyhed(glm::vec3(0, 0, -1), glm::vec3(0, 1, -2), glm::vec3(1, -1, -2), glm::vec3(-1, -1, -2));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(1, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(0, 0, 1), glm::vec3(0, 1, 2), glm::vec3(1, -1, 2), glm::vec3(-1, -1, 2));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(1, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(0, 0, 2), glm::vec3(0, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(1, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(3.5, 0, 1), glm::vec3(4, 0, 0), glm::vec3(3, -1, 0), glm::vec3(3, 1, 0));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(1, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(-3.5, 0, 1), glm::vec3(-4, 0, 0), glm::vec3(-3, -1, 0), glm::vec3(-3, 1, 0));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(1, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(3.5, 0, 2), glm::vec3(4, 0, 1), glm::vec3(3, -1, 1), glm::vec3(3, 1, 1));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(sqrt(2), poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(3.5, 0, 1), glm::vec3(4, 0, 0), glm::vec3(2, -1, 0), glm::vec3(2, 1, 0));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(0, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(3.5, 0, 1), glm::vec3(4, 0, 0), glm::vec3(1, -1, 0), glm::vec3(1, 1, 0));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(0, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(3.5, 0, 1), glm::vec3(4, 0, -1), glm::vec3(3, -1, -1), glm::vec3(3, 1, 0));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(1, poly.dist(polyhed));

    polyhed = gmh::Polyhedron(glm::vec3(0, 0, 1), glm::vec3(0, 1, -2), glm::vec3(1, -1, -2), glm::vec3(-1, -1, -2));
    ASSERT_FLOAT_EQ(polyhed.dist(poly), poly.dist(polyhed));
    EXPECT_FLOAT_EQ(0, poly.dist(polyhed));
}
