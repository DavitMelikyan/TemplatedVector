#include <gtest/gtest.h>
#include "../headers/vector.hpp"

TEST(CtorTest, DefaultCtor) {
    MyVector<int> vec;
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
}

TEST(CtorTest, SizeValCtor) {
    MyVector<int> vec(5, 15);
    EXPECT_EQ(vec.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(vec[i], 15);
    }
}

TEST(CtorTest, CopyCtor) {
    MyVector<int> vec1(10, 5);
    MyVector<int> vec2(vec1);
    EXPECT_EQ(vec1, vec2);
}

TEST(CtorTest, MoveCtor) {
    MyVector<int> v1(10, 5);
    MyVector<int> v2(std::move(v1));
    EXPECT_EQ(v2.size(), 10);
    EXPECT_TRUE(v1.empty());
}

TEST(CtorTest, InitList) {
    MyVector<int> vec;
    vec = {1, 2, 3, 4, 5};
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[3], 4);
}

TEST(ElemAccessTest, AtCheck) {
    MyVector<int> vec(2, 4);
    EXPECT_EQ(vec.at(1), 4);
    EXPECT_THROW(vec.at(3), std::out_of_range);
}

TEST(ElemAccessTest, FrontBackSubscript) {
    MyVector<int> vec (5);
    vec = {10, 20, 30, 40, 50};
    EXPECT_EQ(vec.front(), 10);
    EXPECT_EQ(vec.back(), 50);
    EXPECT_EQ(vec[3], 40);
}

TEST(CapacTest, ResTest) {
    MyVector<int> vec;
    vec.reserve(5);
    EXPECT_EQ(vec.capacity(), 5);
}

TEST(CapacTest, ResizeTest) {
    MyVector<int> vec(5, 10);
    vec.resize(7, 7);
    EXPECT_EQ(vec.size(), 7);
    EXPECT_EQ(vec[6], 7);
    vec.resize(3);
    EXPECT_EQ(vec.size(), 3);
}

TEST(ModifTest, PushPopBack) {
    MyVector<int> vec;
    int a = 10;
    vec.push_back(a);
    vec.push_back(a);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.back(), 10);
    vec.pop_back();
    EXPECT_EQ(vec.size(), 1);
}

TEST(ModifTest, Insert) {
    MyVector<int> vec (2);
    vec = {1, 3};
    vec.insert(1, 2);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[1], 2);
}

TEST(ModifTest, InsertCount) {
    MyVector<int> vec;
    vec = {8, 4};
    vec.insert(1, 3, 7);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_EQ(vec[1], 7);
    EXPECT_EQ(vec[2], 7);
    EXPECT_EQ(vec[3], 7);
}

TEST(ModifTest, EmplaceBack) {
    MyVector<std::string> vec;
    vec.emplace_back("hello");
    vec.emplace_back("world");
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], "world");
}

TEST(ModifTest, Swap) {
    MyVector<int> vec1 (2);
    MyVector<int> vec2 (3);
    vec1 = {1, 2};
    vec2 = {9, 8, 7};
    vec1.swap(vec2);
    EXPECT_EQ(vec1.size(), 3);
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec1[1], 8);
    EXPECT_EQ(vec2[1], 2);
}

TEST(ComparTest, Compare) {
    MyVector<int> vec1 (3);
    vec1 = {1, 2, 3};
    MyVector<int> vec2 (3);
    vec2 = {1, 2, 3};
    MyVector<int> vec3 (2);
    vec3 = {1, 3};
    MyVector<int> vec4 (1);
    vec4 = {10};

    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
    EXPECT_TRUE(vec3 < vec4);
    EXPECT_TRUE(vec3 > vec1);
    EXPECT_TRUE(vec1 <= vec4);
    EXPECT_TRUE(vec1 >= vec2);
}
