#include <gtest/gtest.h>
#include "Graphics/geometry.hpp"
#include "Graphics/hash.hpp"

struct GeoInitTest: public ::testing::Test {
    Point p;
    Line lin;
    LinSeg lin_seg;
    Plane plan;
    Polygon poly;
    Polyhedron polyhed;
    std::vector<std::shared_ptr<Point>> points;

    virtual void SetUp() override {
        points.emplace_back(std::make_shared<Point>(glm::vec3(0, 0, 0)));
        points.emplace_back(std::make_shared<Point>(glm::vec3(1, 0, 0)));
        points.emplace_back(std::make_shared<Point>(glm::vec3(0, 1, 0)));
        points.emplace_back(std::make_shared<Point>(glm::vec3(1, 1, 0)));
        points.emplace_back(std::make_shared<Point>(glm::vec3(0.5, 0.5, 1)));
    }
};

TEST_F(GeoInitTest, DefaultConstructor){
    EXPECT_EQ(0, p.vertices.size());
    EXPECT_EQ(2, lin.vertices.size());
    EXPECT_EQ(2, lin_seg.vertices.size());
    EXPECT_EQ(3, plan.vertices.size());
    EXPECT_EQ(3, poly.vertices.size());
    EXPECT_EQ(3, poly.edges.size());
    EXPECT_EQ(4, polyhed.vertices.size());
    EXPECT_EQ(6, polyhed.edges.size());
    EXPECT_EQ(4, polyhed.faces.size());

    EXPECT_EQ(glm::vec3(0, 0, 0), p.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), p.vel);
    EXPECT_EQ(glm::vec3(0, 0, 0), lin.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), lin.vel);
    EXPECT_EQ(glm::vec3(1, 0, 0), lin.dirVec());
    EXPECT_EQ(glm::vec3(0, 0, 0), lin_seg.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), lin_seg.vel);
    EXPECT_EQ(glm::vec3(1, 0, 0), lin_seg.dirVec());
    EXPECT_EQ(glm::vec3(0, 0, 0), plan.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), plan.vel);
    EXPECT_EQ(glm::vec3(0, 0, 1), plan.normVec());
    EXPECT_EQ(glm::vec3(1.0/3.0, 1.0/3.0, 1.0/3.0), poly.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), poly.vel);
    EXPECT_EQ(glm::vec3(0, 0, 1), poly.normVec());
    for(int i = 0; i < poly.edges.size(); i++){
        EXPECT_LT(0, glm::dot(poly.normVec(), glm::cross(poly.edges[i]->dirVec(), poly.edges[(i+1)%poly.edges.size()]->dirVec())));
    }
    EXPECT_EQ(glm::vec3(0.25, 0.25, 0.25), polyhed.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), polyhed.vel);
    for(std::shared_ptr<Polygon> face: polyhed.faces){
        EXPECT_LT(0, face->sign_dist(Point(polyhed.pos)));
    }
}

TEST_F(GeoInitTest, FailCases){
    EXPECT_THROW(Line(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), std::invalid_argument);

    EXPECT_THROW(LinSeg(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)), std::invalid_argument);

    EXPECT_THROW(Plane(glm::vec3(1, 0, 0), glm::vec3(2, 0, 0), glm::vec3(3, 0, 0)), std::invalid_argument);

    EXPECT_THROW(Polygon(glm::vec3(1, 0, 0), glm::vec3(2, 0, 0), glm::vec3(3, 0, 0)), std::invalid_argument);
    EXPECT_THROW(Polygon(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 0)), std::invalid_argument);
    EXPECT_THROW(Polygon(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0.01)), std::invalid_argument);
    EXPECT_THROW(Polygon(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0, 0)), std::invalid_argument);
    EXPECT_THROW(Polygon(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 1, 0)), std::invalid_argument);

    EXPECT_THROW(Polyhedron(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0)), std::invalid_argument);
    EXPECT_THROW(Polyhedron(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(0.5, 0.5, 0.5)), std::invalid_argument);
    EXPECT_THROW(Polyhedron(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(0.5, 0.5, 0)), std::invalid_argument);
    EXPECT_THROW(Polyhedron(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(1, 0, 0)), std::invalid_argument);
    EXPECT_THROW(Polyhedron(glm::vec3(0, 0, 0), glm::vec3(2, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0.5, 0.5, 1), glm::vec3(1, 1, 0)), std::invalid_argument);
}

TEST_F(GeoInitTest, PointInit){
    p = Point({1, 1, 1});
    EXPECT_EQ(glm::vec3(1, 1, 1), p.pos);
    EXPECT_EQ(glm::vec3(0, 0, 0), p.vel);
    EXPECT_EQ(0, p.dim());
    EXPECT_TRUE(p.isSpace());
    EXPECT_NE(Point({0, 0, 0}), p);
}

TEST_F(GeoInitTest, LineInit){
    lin = Line(points[0], points[1]);

    EXPECT_EQ(2, lin.vertices.size());
    EXPECT_EQ(1, lin.dim());
    EXPECT_TRUE(lin.isSpace());
    EXPECT_EQ(glm::vec3(1, 0, 0), lin.dirVec());
    EXPECT_EQ(Line(*points[0], *points[1]), lin);
    EXPECT_EQ(Line(std::vector<Point>{*points[0], *points[1]}), lin);
    EXPECT_EQ(Line(std::vector<std::shared_ptr<Point>>{points[0], points[1]}), lin);
    EXPECT_NE(Line(points[0], points[2]), lin);
}

TEST_F(GeoInitTest, LinSegInit){
    lin_seg = LinSeg(points[0], points[1]);

    EXPECT_EQ(2, lin_seg.vertices.size());
    EXPECT_EQ(1, lin_seg.dim());
    EXPECT_FALSE(lin_seg.isSpace());
    EXPECT_FLOAT_EQ(1, lin_seg.length());
    EXPECT_EQ(glm::vec3(1, 0, 0), lin_seg.dirVec());
    EXPECT_EQ(LinSeg(*points[0], *points[1]), lin_seg);
    EXPECT_EQ(LinSeg(std::vector<Point>({*points[0], *points[1]})), lin_seg);
    EXPECT_EQ(LinSeg(std::vector<std::shared_ptr<Point>>({points[0], points[1]})), lin_seg);
    EXPECT_NE(LinSeg(points[0], points[2]), lin_seg);
}

TEST_F(GeoInitTest, PlaneInit){
    plan = Plane(points[0], points[1], points[2]);

    EXPECT_EQ(3, plan.vertices.size());
    EXPECT_EQ(2, plan.dim());
    EXPECT_TRUE(plan.isSpace());
    EXPECT_EQ(glm::vec3(0, 0, 1), plan.normVec());
    EXPECT_EQ(Plane(*points[0], *points[1], *points[2]), plan);
    EXPECT_EQ(Plane(std::vector<Point>({*points[0], *points[1], *points[2]})), plan);
    EXPECT_EQ(Plane(std::vector<std::shared_ptr<Point>>({points[0], points[1], points[2]})), plan);
    EXPECT_NE(Plane(points[0], points[1], points[3]), plan);
}

TEST_F(GeoInitTest, PolygonInit){
    poly = Polygon(points[0], points[1], points[2], points[3]);

    EXPECT_EQ(points[0], poly.vertices[0]);
    EXPECT_EQ(points[1], poly.vertices[1]);
    EXPECT_EQ(points[2], poly.vertices[3]);
    EXPECT_EQ(points[3], poly.vertices[2]);

    EXPECT_EQ(LinSeg(points[0], points[1]), *poly.edges[0]);
    EXPECT_EQ(LinSeg(points[1], points[3]), *poly.edges[1]);
    EXPECT_EQ(LinSeg(points[3], points[2]), *poly.edges[2]);
    EXPECT_EQ(LinSeg(points[2], points[0]), *poly.edges[3]);

    for(int i = 0; i < poly.edges.size(); i++){
        EXPECT_LT(0, glm::dot(poly.normVec(), glm::cross(poly.edges[i]->dirVec(), poly.edges[(i+1)%poly.edges.size()]->dirVec())));
    }

    EXPECT_EQ(4, poly.vertices.size());
    EXPECT_EQ(4, poly.edges.size());
    EXPECT_EQ(2, poly.dim());
    EXPECT_FALSE(poly.isSpace());
    EXPECT_FLOAT_EQ(1, poly.area());
    EXPECT_EQ(Polygon(*points[0], *points[1], *points[2], *points[3]), poly);
    EXPECT_EQ(Polygon(std::vector<Point>({*points[0], *points[1], *points[2], *points[3]})), poly);
    EXPECT_EQ(Polygon(std::vector<std::shared_ptr<Point>>({points[0], points[1], points[2], points[3]})), poly);
    EXPECT_NE(Polygon(points[0], points[1], points[2]), poly);
}

TEST_F(GeoInitTest, PolyhedronInit){
    polyhed = Polyhedron(points[0], points[1], points[2], points[3], points[4]);

    EXPECT_EQ(5, polyhed.vertices.size());
    EXPECT_EQ(8, polyhed.edges.size());
    EXPECT_EQ(5, polyhed.faces.size());
    EXPECT_EQ(3, polyhed.dim());
    EXPECT_FALSE(polyhed.isSpace());
    EXPECT_FLOAT_EQ(1.0F/3.0F, polyhed.volume());
    EXPECT_EQ(Polyhedron(*points[0], *points[1], *points[2], *points[3], *points[4]), polyhed);
    EXPECT_EQ(Polyhedron(std::vector<Point>({*points[0], *points[1], *points[2], *points[3], *points[4]})), polyhed);
    EXPECT_EQ(Polyhedron(points), polyhed);
    EXPECT_NE(Polyhedron(points[0], points[1], points[2], points[4]), polyhed);
    for(std::shared_ptr<Polygon> face: polyhed.faces){
        EXPECT_LT(0, face->sign_dist(Point(polyhed.pos)));
    }
}
