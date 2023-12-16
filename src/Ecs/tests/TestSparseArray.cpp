/*
** EPITECH PROJECT, 2023
** Bootstrap
** File description:
** TestSparseArray
*/

#include <criterion/criterion.h>
#include "../SparseArray.hpp"

Test(insert_at, test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    cr_assert_eq(1, spa[0]);
}

Test(size, test_basic) {
    sparse_array<int> spa;

    cr_assert_eq(0, spa.size());
    spa.insert_at(0, 1);
    cr_assert_eq(1, spa.size());
}

Test(emplace_at, test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    cr_assert_eq(1, spa[0]);
    spa.emplace_at(0, 2);
    cr_assert_eq(2, spa[0]);
}

Test(erase, test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    spa.insert_at(0, 2);
    spa.erase(0);
    cr_assert_eq(2, spa[0]);
}

Test(get_index, test_basic) {
    sparse_array<int> spa;
    spa.insert_at(0, 10);
    spa.insert_at(1, 20);
    spa.insert_at(2, 30);

    int valueToFind = 20;

    sparse_array<int>::size_type index = spa.get_index(valueToFind);
    cr_assert_eq(1, index);
}

Test(get_index, error_test_basic) {
    sparse_array<int> spa;
    spa.insert_at(0, 10);
    spa.insert_at(1, 20);
    spa.insert_at(2, 30);

    int valueToFind = 40;

    sparse_array<int>::size_type index = spa.get_index(valueToFind);
    cr_assert_eq(-1, index);
}

Test(erase, error_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    try {
        spa.erase(3);
    } catch (const std::exception &e) {
        cr_assert_eq(true, true);
    }
}

Test(emplace_at, error_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    cr_assert_eq(1, spa[0]);
    try {
        spa.emplace_at(-2, 2);
    } catch (const std::exception &e) {
        cr_assert_eq(true, true);
    }
}

Test(insert_at, error_test_basic) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    cr_assert_eq(1, spa[0]);
}

Test(insert_at, multiple_places) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    spa.insert_at(4, 2);
    spa.insert_at(10, 3);

    cr_assert_eq(1, spa[0]);
    cr_assert_eq(2, spa[4]);
    cr_assert_eq(3, spa[10]);
}

Test(copy_construct, copy_constructor_good_params) {
    sparse_array<int> spa;

    spa.insert_at(0, 1);
    spa.insert_at(4, 2);
    spa.insert_at(10, 3);

    sparse_array<int> spa1(spa);
    cr_assert_eq(spa1[0], spa[0]);
    cr_assert_eq(spa1[4], spa[4]);
    cr_assert_eq(spa1[10], spa[10]);
}

Test(index_too_far, too_far_index) {
    sparse_array<int> spa;
    bool error = false;

    spa.insert_at(0, 1);
    try {
        spa[50];
    } catch (std::exception) {
        error = true;
    }

    cr_assert_eq(error, true);
}
