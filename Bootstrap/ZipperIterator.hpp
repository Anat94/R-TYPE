/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ZipperIterator
*/

#ifndef ZIPPERITERATOR_HPP
    #define ZIPPERITERATOR_HPP
    #include <vector>
    #include <memory>
    #include <optional>
    #include <iostream>
    #include <algorithm>
    #include <iterator>

template <class... Containers>
class zipper_iterator {
    // Define types for iterator and reference
    template <class Container>
    using iterator_t = decltype(std::declval<Container>().begin());
    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;
    public:
        using value_type = std::tuple<it_reference_t<Containers>...>;
        using reference = value_type;
        using pointer = void;
        using difference_type = size_t;
        using iterator_category = std::input_iterator_tag;
        using iterator_tuple = std::tuple<iterator_t<Containers>...>;
        // friend zipper<Containers...>;
        zipper_iterator(const iterator_tuple& it_tuple, size_t max, size_t idx): _current(it_tuple), _max(max), _idx(idx) {}
        zipper_iterator(const zipper_iterator& z): _current(z._current), _max(z._max), _idx(z._idx) {}
        zipper_iterator operator++() {
            if (_idx > _max)
                throw std::out_of_range("😉");
            ++_idx;
            incr_all(_seq);
            return *this;
        }
        zipper_iterator& operator++(int) {
            zipper_iterator temp = *this;
            ++(*this);
            return temp;
        }
        value_type operator*() {
            return to_value(_seq);
        }
        value_type operator->() {
            return to_value(_seq);
        }
        friend bool operator==(const zipper_iterator& lhs, const zipper_iterator& rhs) {
            return (lhs._idx == rhs._idx) && (lhs._max == rhs._max);
        }

        friend bool operator!=(const zipper_iterator& lhs, const zipper_iterator& rhs) {
            return !(lhs == rhs);
        }
    private:
        template <size_t... Is>
        void incr_all(std::index_sequence<Is...>) {
            (++std::get<Is>(_current), ...);
        }

        template <size_t... Is>
        bool all_set(std::index_sequence<Is...>) {
            return (std::get<Is>(_current)[_idx].has_value() && ...);
        }

        template <size_t... Is>
        value_type to_value(std::index_sequence<Is...>) {
            return value_type(*(std::get<Is>(_current))...);
        }
    private:
        iterator_tuple _current;
        size_t _max;
        size_t _idx;
        static constexpr std::index_sequence_for<Containers...> _seq{};
};

template <class... Containers>
class zipper {
    public:
        using iterator = zipper_iterator<Containers...>;
        using iterator_tuple = typename iterator::iterator_tuple;

        zipper(Containers&... cs): _begin(cs.begin()...), _end(_compute_end(cs...)), _size(_compute_size(cs...)) {}

        iterator begin() {
            return iterator(_begin, _size, 0); }
        iterator end() { return iterator(_end, _size, _size); }
    private:
        static size_t _compute_size(Containers&... containers) {
            return std::min({containers.size()...});
        }

        static iterator_tuple _compute_end(Containers&... containers) {
            return iterator_tuple(containers.end()...);
        }
    private:
        iterator_tuple _begin;
        iterator_tuple _end;
        size_t _size;
};

#endif // ZIPPERITERATOR_HPP
