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

/**
 * @brief Zipper Iterator class usefull for looping through multiple sparse arrays at the same time.
*/
template <class... Containers>
class zipper_iterator {
    // Define types for iterator and reference
    template <class Container>
    using iterator_t = decltype(std::declval<Container>().begin());
    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference;
    public:
        using value_type = std::tuple<size_t, it_reference_t<Containers>...>;
        using reference = value_type;
        using pointer = void;
        using difference_type = size_t;
        using iterator_category = std::input_iterator_tag;
        using iterator_tuple = std::tuple<iterator_t<Containers>...>;
        // friend zipper<Containers...>;
        /**
         * @brief Construct a zipper iterator from an iterator tuple
         * 
         * @param it_tuple tuple containing iterators for the templated containers
         * @param max maximum size of the iterator range
         * @param idx current index of the iterator_tuple previously passed in parameter
         */
        zipper_iterator(const iterator_tuple& it_tuple, size_t max, size_t idx): _current(it_tuple), _max(max), _idx(idx) {}
        /**
         * @brief Copy constructor for the zipper_iterator
         * 
         * @param z another zipper_iterator
         */
        zipper_iterator(const zipper_iterator& z): _current(z._current), _max(z._max), _idx(z._idx) {}
        /**
         * @brief Incrementor for the zipper iterator, increments all iterators in memory by 1
         * 
         * @returns current zipper_iterator object
         */
        zipper_iterator operator++() {
            if (_idx > _max)
                throw std::out_of_range("😉");
            ++_idx;
            incr_all(_seq);
            return *this;
        }
        /**
         * @brief Incrementor for the zipper iterator, increments all iterators in memory by 1
         * 
         * @returns zipper_iterator object before the operation
         */
        zipper_iterator& operator++(int) {
            zipper_iterator temp = *this;
            ++(*this);
            return temp;
        }
        /**
         * @brief pointer operation on zipper_iterator
         * 
         * @returns tuple of values corresponding to the associated iterators of each container
         */
        value_type operator*() {
            return to_value(_seq);
        }
        /**
         * @brief pointer operation on zipper_iterator
         * 
         * @returns tuple of values corresponding to the associated iterators of each container
         */
        value_type operator->() {
            return to_value(_seq);
        }
        /**
         * @brief assertion operator on 2 zipper iterators
         * 
         * @param lhs first zipper_iterator to compare
         * @param rhs second zipper_iterator to compare
         * 
         * @returns true if the two zipper_iterators are at the same index and contain the same values, false otherwise
         */
        friend bool operator==(const zipper_iterator& lhs, const zipper_iterator& rhs) {
            return (lhs._idx == rhs._idx) && (lhs._max == rhs._max);
        }

        /**
         * @brief not equal assertion operator on 2 zipper iterators
         * 
         * @param lhs first zipper_iterator to compare
         * @param rhs second zipper_iterator to compare
         * 
         * @returns false if the two zipper_iterators are at the same index and contain the same values, true otherwise
         */
        friend bool operator!=(const zipper_iterator& lhs, const zipper_iterator& rhs) {
            return !(lhs == rhs);
        }
    private:
        /**
         * @brief zipper_iterator incrementor, increments all individual container iterators at once
         * 
         * @param Is index_sequence corresponding to the tuple indices to increment
         * 
         */
        template <size_t... Is>
        void incr_all(std::index_sequence<Is...>) {
            (++std::get<Is>(_current), ...);
        }

        /**
         * @brief checks if all values at current iterator position are set
         * 
         * @param Is index_sequence corresponding to the tuple indices to check
         * 
         * @returns true if all values of std::optional at the current iterator position are set, false otherwise
         */
        template <size_t... Is>
        bool all_set(std::index_sequence<Is...>) {
            return (std::get<Is>(_current)[_idx].has_value() && ...);
        }

        /**
         * @brief converts the zipper_iterator, to a tuple of values corresponding to the indices of the current iterators
         * 
         * @param Is index_sequence corresponding to the tuple indices to convert to values
         * 
         * @returns a tuple containing Container values at corresponding indices of the current iterators
         */
        template <size_t... Is>
        value_type to_value(std::index_sequence<Is...>) {
            return value_type(_idx, *(std::get<Is>(_current))...);
        }
    private:
        /**
         * @brief current tuple containing all iterators from templated containers
        */
        iterator_tuple _current;
        /**
         * @brief maximum index to which the iterators can iterate up to
        */
        size_t _max;
        /**
         * @brief current index of the iterators
        */
        size_t _idx;
        /**
         * @brief index sequence containing necessary indices from 0 to the iterator_tuple length
        */
        static constexpr std::index_sequence_for<Containers...> _seq{};
};

/**
 * @brief zipper class, allows you to zip Containers into a tuple of iterators, allows you to use them in range based for loops
*/
template <class... Containers>
class zipper {
    public:
        using iterator = zipper_iterator<Containers...>;
        using iterator_tuple = typename iterator::iterator_tuple;

        /**
         * @brief zipper constructor, constructs a tuple containing iterators for each container passed as parameter
         * 
         * @param cs All the containers to zip
        */
        zipper(Containers&... cs): _begin(cs.begin()...), _end(_compute_end(cs...)), _size(_compute_size(cs...)) {}

        /**
         * @brief get the beginning iterators of the zipper
         * 
         * @returns tuple containing the beginning iterators of each container in the zipper
        */
        iterator begin() {
            return iterator(_begin, _size, 0); }
        /**
         * @brief get the end iterators of the zipper
         * 
         * @returns tuple containing the end iterators of each container in the zipper
        */
        iterator end() { return iterator(_end, _size, _size); }
    private:
        /**
         * @brief computes the maximum size of a list of Containers
         * 
         * @param containers containers to compute
         * 
         * @returns maximum size of the containers
        */
        static size_t _compute_size(Containers&... containers) {
            return std::min({containers.size()...});
        }

        /**
         * @brief computes the end of a list of Containers, returns an iterator tuple containing the end iterator of each container
         * 
         * @param containers containers to compute
         * 
         * @returns iterator_tuple containing the end iterator of each container
        */
        static iterator_tuple _compute_end(Containers&... containers) {
            return iterator_tuple(containers.end()...);
        }
    private:
        /**
         * @brief iterator tuple representing the beginning iterators of the zipper.
        */
        iterator_tuple _begin;
        /**
         * @brief iterator tuple representing the end iterators of the zipper.
        */
        iterator_tuple _end;
        /**
         * @brief maximum index up to which the iterators can iterate up to.
        */
        size_t _size;
};

#endif // ZIPPERITERATOR_HPP
