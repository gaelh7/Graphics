#include <gtest/gtest.h>
#include <Graphics/geometry.hpp>

struct PointInterTest: public ::testing::Test {
    gmh::Point obj;
    virtual void SetUp() override {
        obj = glm::vec3({0, 0, 0});
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
        EXPECT_TRUE(obj.intersect(obj2)->equals(*obj2.intersect(obj))) << "obj.intersect(obj2): " << *obj.intersect(obj2) << std::endl << "obj.intersect(obj2): " << *obj2.intersect(obj);
        EXPECT_TRUE(obj.intersect(obj2)->equals(inter)) << "Expected Intersection: " << inter << std::endl << "Actual Intersection: " << *obj.intersect(obj2);
        if(HasFailure()) std::cout << "obj: " << obj << std::endl << "obj2: " << obj2 << std::endl;
    }
};

TEST_F(PointInterTest, PointInterWithPoint){
    gmh::Point p2({1, 1, 0});

    checkNoInter(p2);

    obj.pos = {1, 1, 0};
    checkYesInter(p2, gmh::Point({1, 1, 0}));
}

TEST_F(PointInterTest, PointInterWithLine){
    gmh::Line lin(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0));

    checkNoInter(lin);

    obj.pos.y = -999;
    checkNoInter(lin);

    obj.pos.y = 1001;
    checkNoInter(lin);

    obj.pos = {100, 1, 0};
    checkYesInter(lin, gmh::Point({100, 1, 0}));
}

TEST_F(PointInterTest, PointInterWithLinSegNoClamp){
    gmh::LinSeg lin(glm::vec3(-1, 1, 0), glm::vec3(1, 1, 0));

    checkNoInter(lin);

    obj.pos.y = -999;
    checkNoInter(lin);

    obj.pos.y = 1001;
    checkNoInter(lin);

    obj.pos = {0.5, 1, 0};
    checkYesInter(lin, gmh::Point({0.5, 1, 0}));
}

TEST_F(PointInterTest, PointInterWithLinSegWithClamp){
    gmh::LinSeg lin1(glm::vec3(100, 1, 0), glm::vec3(101, 1, 0));
    gmh::LinSeg lin2(glm::vec3(-100, 1, 0), glm::vec3(-101, 1, 0));

    checkNoInter(lin1);

    obj.pos.y = -99;
    checkNoInter(lin1);

    obj.pos.y = 101;
    checkNoInter(lin1);
}

TEST_F(PointInterTest, PointInterWithPlane){
    gmh::Plane plan(glm::vec3(0, 1, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 1));

    checkNoInter(plan);

    obj.pos.y = 5;
    checkNoInter(plan);

    obj.pos = {-4.5e5, 1, 8e2};
    checkYesInter(plan, gmh::Point({-4.5e5, 1, 8e2}));
}

TEST_F(PointInterTest, PointInterWithPolygonInPlane){
    gmh::Polygon poly(glm::vec3(1, 0, 0), glm::vec3(2, -1, 0), glm::vec3(2, 1, 0));

    checkNoInter(poly);

    obj.pos.x = 5;
    checkNoInter(poly);

    obj.pos = {2, 3, 0};
    checkNoInter(poly);

    obj.pos = {1, 1, 0};
    checkNoInter(poly);

    obj.pos = {1.5, 0, 0};
    checkYesInter(poly, gmh::Point({1.5, 0, 0}));
}

TEST_F(PointInterTest, PointInterWithPolygonNoClamp){
    gmh::Polygon poly(glm::vec3(-1, 0, 1), glm::vec3(1, -2, 1), glm::vec3(1, 2, 1));

    checkNoInter(poly);

    obj.pos.z = 2;
    checkNoInter(poly);

    obj.pos.x = -1;
    checkNoInter(poly);

    obj.pos.x = 1;
    checkNoInter(poly);
}

TEST_F(PointInterTest, PointInterWithPolygonWithClamp){
    gmh::Polygon poly(glm::vec3(-3, 0, 1), glm::vec3(-1, -2, 1), glm::vec3(-1, 2, 1));

    checkNoInter(poly);

    obj.pos.z = 2;
    checkNoInter(poly);

    obj.pos.y = 3;
    checkNoInter(poly);

    obj.pos.x = -1;
    checkNoInter(poly);
}

TEST_F(PointInterTest, PointInterWithPolyhedron){
    gmh::Polyhedron poly(glm::vec3(1, 0, 0), glm::vec3(2, -1, 0), glm::vec3(2, 1, 0), glm::vec3(1.5, 0, 1));

    checkNoInter(poly);

    obj.pos = {1.5, 0, -1};
    checkNoInter(poly);

    obj.pos.z = 2;
    checkNoInter(poly);

    obj.pos = {1, 0, 1};
    checkNoInter(poly);

    obj.pos = {1.5, 0, 0.5};

    checkYesInter(poly, gmh::Point({1.5, 0, 0.5}));
}
