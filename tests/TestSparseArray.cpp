#include <gtest/gtest.h>
#include "../src/Ecs/SparseArray.hpp"

TEST(TestSparseArray, insert_at_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    EXPECT_EQ(1, spa[0]);
}

TEST(TestSparseArray, size_test_basic) {
    sparse_array<int> spa;

    EXPECT_EQ(0, spa.size());
    spa.insert_at(0, 1);
    EXPECT_EQ(1, spa.size());
}

TEST(TestSparseArray, emplace_at_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    EXPECT_EQ(1, spa[0]);
    spa.emplace_at(0, 2);
    EXPECT_EQ(2, spa[0]);
}

TEST(TestSparseArray, erase_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    spa.insert_at(0, 2);
    spa.erase(0);
    EXPECT_EQ(false, spa[0].has_value());
}

TEST(TestSparseArray, get_index_test_basic) {
    sparse_array<int> spa;
    spa.insert_at(0, 10);
    spa.insert_at(1, 20);
    spa.insert_at(2, 30);

    int valueToFind = 20;

    sparse_array<int>::size_type index = spa.get_index(valueToFind);
    EXPECT_EQ(1, index);
}

TEST(TestSparseArray, get_index_error_test_basic) {
    sparse_array<int> spa;
    spa.insert_at(0, 10);
    spa.insert_at(1, 20);
    spa.insert_at(2, 30);

    int valueToFind = 40;

    sparse_array<int>::size_type index = spa.get_index(valueToFind);
    EXPECT_EQ(-1, index);
}

TEST(TestSparseArray, erase_error_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    EXPECT_THROW(spa.erase(3), std::exception);
}

TEST(TestSparseArray, emplace_at_error_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    EXPECT_EQ(1, spa[0]);
    EXPECT_THROW(spa.emplace_at(-2, 2), std::exception);
}

TEST(TestSparseArray, insert_at_error_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    EXPECT_EQ(1, spa[0]);
}

TEST(TestSparseArray, insert_at_multiple_places) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    spa.insert_at(4, 2);
    spa.insert_at(10, 3);

    EXPECT_EQ(1, spa[0]);
    EXPECT_EQ(2, spa[4]);
    EXPECT_EQ(3, spa[10]);
}

TEST(TestSparseArray, copy_construct_copy_constructor_good_params) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    spa.insert_at(4, 2);
    spa.insert_at(10, 3);

    sparse_array<int> spa1(spa);
    EXPECT_EQ(spa1[0], spa[0]);
    EXPECT_EQ(spa1[4], spa[4]);
    EXPECT_EQ(spa1[10], spa[10]);
}

TEST(TestSparseArray, index_too_far_too_far_index_no_crash) {
    sparse_array<int> spa;
    bool error = false;

    spa.insert_at(0, 1);
    try {
        spa[50];
    } catch (std::exception) {
        error = true;
    }

    EXPECT_EQ(error, false);
}
