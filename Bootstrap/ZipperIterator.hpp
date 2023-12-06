/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ZipperIterator
*/

template <class ... Containers>
class zipper_iterator {
    template <class Container>
    using iterator_t = ??? // type of Container :: begin () return value
    template <class Container>
    using it_reference_t = typename iterator_t<Container>::reference ;
    public :
        using value_type = std :: tuple </* retrieve it_reference_t :: value () type */&...>; // std :: tuple of references to components
        using reference = value_type ;
        using pointer = void;
        using difference_type = size_t;
        using iterator_category = /* proper iterator tag */;
        using iterator_tuple = std::tuple<iterator_t<Container>...>;
        // If we want zipper_iterator to be built by zipper only .
        friend containers::zipper<Containers ...>;
        zipper_iterator(iterator_tuple const &it_tuple, size_t max);
    public:
        zipper_iterator(zipper_iterator const &z);
        zipper_iterator operator++();
        zipper_iterator &operator++(int);
        value_type operator*();
        value_type operator->();
        friend bool operator ==(zipper_iterator const &lhs, zipper_iterator const &rhs);
        friend bool operator !=(zipper_iterator const &lhs, zipper_iterator const &rhs);
    private :
        // Increment every iterator at the same time . It also skips to the next value if one of the pointed to std :: optional does not contains a value .
        template <size_t ...Is>
        void incr_all ( std :: index_sequence<Is ...>);
        // check if every std :: optional are set .
        template <size_t ...Is>
        bool all_set(std::index_sequence<s ...>);
        // return a tuple of reference to components .
        template <size_t ...Is>
        value_type to_value (std::index_sequence<Is ...>);
    private :
        iterator_tuple _current;
        size_t _max; // compare this value to _idx to prevent infinite loop .
        size_t _idx;
        static constexpr std::index_sequence_for<Containers...> _seq {};
};