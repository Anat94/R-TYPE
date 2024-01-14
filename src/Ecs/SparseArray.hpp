/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** SparseArray
*/

#ifndef SPARSEARRAY_HPP
    #define SPARSEARRAY_HPP
    //#pragma warning(disable: 4668)
    //#pragma warning(disable: 4626)
    //#pragma warning(disable: 4625)
    //#pragma warning(disable: 4820)
//#pragma warning(disable: 5031)
//#pragma warning(disable: 4365)
//#pragma warning(disable: 5027)
//#pragma warning(disable: 4514)
//#pragma warning(disable: 4464)
//#pragma warning(disable: 5026)
//#pragma warning(disable: 4457)
//#pragma warning(disable: 5262)
//#pragma warning(disable: 5204)
//#pragma warning(disable: 4355)
//#pragma warning(disable: 5220)
//#pragma warning(disable: 5039)
    #include <vector>
    #include <memory>
    #include <optional>
    #include <iostream>
    #include <algorithm>
    #include <utility>

/// @brief This class allows you to store any value into a list of the `Component`.
/// @tparam Component the type of the object you want to add in your `sparse_array` object
template <typename Component> // Type of the value you want to store in the `sparse_array` class.
class sparse_array {
    public:
        /// @brief Type of the value stored in the `sparse_array` object.
        using value_type = std::optional<Component>;
        /// @brief Reference type of the value stored in the `sparse_array` object.
        using reference_type = value_type &;
        /// @brief Const reference type of the value stored in the `sparse_array` object.
        using const_reference_type = value_type const &;
        /// @brief Container used to store the values in the `sparse_array` object.
        using container_t = std::vector<value_type>;
        /// @brief Size type of the values stored in the `sparse_array` object.
        using size_type = typename container_t::size_type;
        /// @brief Iterator corresponding to the container's one.
        using iterator = typename container_t::iterator;
        /// @brief Const iterator corresponding to the container's one.
        using const_iterator = typename container_t::const_iterator;
    public:
        /// @brief Default constructor for the `sparse_array` object.
        sparse_array()  = default;
        /// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
        /// @param other The second `sparse_array` object to copy from.
        sparse_array(sparse_array const &other);
        /// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
        /// @param other The second `sparse_array` object to copy from.
        sparse_array(sparse_array &&other) noexcept;
        /// @brief Default destructor for the `sparse_array` object.
        ~sparse_array()  = default;
        /// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
        /// @param other The `sparse_array` object to copy the values from.
        /// @return The current instance of the `sparse_array` object, filled with the values of the `other` object.
        sparse_array &operator=(sparse_array const &other);
        /// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
        /// @param other The `sparse_array` object to copy the values from.
        /// @return The current instance of the `sparse_array` object, filled with the values of the `other` object.
        sparse_array &operator=(sparse_array &&other) noexcept;
        /// @brief Gives you access to the element at the specified index.
        /// @param idx The index where you want to access the value.
        /// @return The value at the specified index.
        reference_type operator[](size_t idx);
        /// @brief Gives you access to the element at the specified index.
        /// @param idx The index where you want to access the value.
        /// @return The value at the specified index.
        const_reference_type operator[](size_t idx) const;
        /**
         * @brief get the beginning iterator of the sparse array
         * 
         * @returns beginning iterator of the sparse_array
         */
        iterator begin();
        /**
         * @brief get the constant beginning iterator of the sparse array
         * 
         * @returns constant beginning iterator of the sparse_array
         */
        const_iterator begin() const;
        /**
         * @brief get the constant beginning iterator of the sparse array
         * 
         * @returns constant beginning iterator of the sparse_array
         */
        const_iterator cbegin() const;
        /**
         * @brief get the end iterator of the sparse array
         * 
         * @returns end iterator of the sparse_array
         */
        iterator end();
        /**
         * @brief get the constant end iterator of the sparse array
         * 
         * @returns constant end iterator of the sparse_array
         */
        const_iterator end() const;
        /**
         * @brief get the constant end iterator of the sparse array
         * 
         * @returns constant end iterator of the sparse_array
         */
        const_iterator cend() const;
        /**
         * @brief resize the sparse array with a new size
         * 
         * @param new_size new wanted size of the sparse_array
         */
        void resize(size_type new_size);
        /**
         * @brief get the size of the sparse_array
         * 
         * @return size of the array
         */
        size_type size() const;
        /**
         * @brief insert in the sparse array at said position
         * 
         * @param pos position to insert in
         * @param value value of component you want to insert
         * @return reference to the sparse_array 
         */
        reference_type insert_at(size_type pos, const Component &value) {
            if (pos >= _data.size())
                _data.resize(pos + 1);
            _data[pos] = value;
            return _data[pos];
        }
        /**
         * @brief insert in the sparse array at said position
         * 
         * @param pos position to insert in
         * @param value value of component you want to insert
         * @return reference to the sparse_array 
         */
        reference_type insert_at(size_type pos, Component &&value) {
            if (pos >= _data.size())
                _data.resize(pos + 1);
            _data[pos] = std::move(value);
            return _data[pos];
        }
        /**
         * @brief 
         * 
         * @tparam Params (Containers classes to add)
         * @param pos position at which to add the components
         * @param params containers to add
         * @return reference of the sparse_array
         */
        template <class ...Params>
        reference_type emplace_at(size_type pos, Params &&...params) {
            if (pos >= _data.size()) {
                _data.resize(pos + 1);
            }
            new (&_data[pos]) Component(std::forward<Params>(params)...);
            return _data[pos];
        }
        /**
         * @brief erase a specific index of the sparse array
         * 
         */
        void erase(size_type pos);
        /**
         * @brief Get the index of a value
         * 
         * @return size_type 
         */
        size_type get_index(value_type const&) const;
    private:
        /**
         * @brief content of the sparse array
         * 
         */
        container_t _data;
};

#include "SparseArray.tpp"

#endif // SPARSEARRAY_HPP