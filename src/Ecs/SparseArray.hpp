/*
** EPITECH PROJECT, 2023
** Bootstrap
** File description:
** SparseArray
*/

#ifndef SPARSEARRAY_HPP
    #define SPARSEARRAY_HPP
    #include <vector>
    #include <memory>
    #include <optional>
    #include <iostream>
    #include <algorithm>
    #include <utility>


template <typename Component> // You can also mirror the definition of std::vector, that takes an additional allocator.
class sparse_array {
    public:
        using value_type = std::optional<Component>; // optional component type
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type>; // optionally add your allocator template here.
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;
    public:
        sparse_array()  = default; // You can add more constructors.
        sparse_array(sparse_array const &other); // copy constructor
        sparse_array(sparse_array &&) noexcept; // move constructor
        ~sparse_array()  = default;
        sparse_array &operator=(sparse_array const &other); // copy assignment operator
        sparse_array &operator=(sparse_array &&other) noexcept; // move assignment operator
        reference_type operator[](size_t idx);
        const_reference_type operator[](size_t idx) const;
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        size_type size() const;
        reference_type insert_at(size_type pos, Component const &);
        reference_type insert_at(size_type pos, Component &&);
        template <class ...Params>
        reference_type emplace_at(size_type pos, Params &&...params);
        void erase(size_type pos);
        size_type get_index(value_type const&) const;
    private:
        container_t _data;
};

#include "SparseArray.tpp"

#endif // SPARSEARRAY_HPP