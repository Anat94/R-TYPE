/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** SparseArray
*/

#ifndef SPARSEARRAY_TPP
    #define SPARSEARRAY_TPP
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


/// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
/// @param other The second `sparse_array` object to copy from.
template <typename Component>
inline sparse_array<Component>::sparse_array(const sparse_array &other) : _data(other._data) {}

/// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
/// @param other The second `sparse_array` object to copy from.
template <typename Component>
inline sparse_array<Component>::sparse_array(sparse_array &&other) noexcept : _data(std::move(other._data)) {}

/// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
/// @param other The `sparse_array` object to copy the values from.
/// @return The current instance of the `sparse_array` object, filled with the values of the `other` object.
template <typename Component>
inline sparse_array<Component> &sparse_array<Component>::operator=(const sparse_array<Component> &other)
{
    if (this != &other)
        _data = other._data;
    return *this;
}

/// @brief Takes the values of the `other` object and copy them into the current `sparse_array` instance.
/// @param other The `sparse_array` object to copy the values from.
/// @return The current instance of the `sparse_array` object, filled with the values of the `other` object.
template <typename Component>
inline sparse_array<Component> &sparse_array<Component>::operator=(sparse_array<Component> &&other) noexcept
{
    if (this != &other)
        _data = std::move(other._data);
    return *this;
}

/// @brief Gives you access to the element at the specified index.
/// @param idx The index where you want to access the value.
/// @return The value at the specified index.
template<class Component>
inline typename sparse_array<Component>::reference_type sparse_array<Component>::operator[](size_t idx)
{
    if (idx >= _data.size()) {
        _data.resize(idx + 1);
    }
    return _data[idx];
}

/// @brief Gives you access to the element at the specified index.
/// @param idx The index where you want to access the value.
/// @return The value at the specified index.
template <typename Component>
inline typename sparse_array<Component>::const_reference_type sparse_array<Component>::operator[](size_t idx) const
{
    if (idx < _data.size() && _data[idx])
        return _data[idx];
    else
        throw std::out_of_range("🙂: out of range for [] but const");
}

/**
 * @brief get the beginning iterator of the sparse array
 * 
 * @returns beginning iterator of the sparse_array
 */
template<class Component>
inline typename sparse_array<Component>::iterator sparse_array<Component>::begin()
{
    return _data.begin();
}

/**
         * @brief get the constant beginning iterator of the sparse array
         * 
         * @returns constant beginning iterator of the sparse_array
         */
template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::begin() const
{
    return _data.begin();
}

/**
         * @brief get the constant beginning iterator of the sparse array
         * 
         * @returns constant beginning iterator of the sparse_array
         */
template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::cbegin() const
{
    return _data.cbegin();
}

/**
         * @brief get the end iterator of the sparse array
         * 
         * @returns end iterator of the sparse_array
         */
template<class Component>
inline typename sparse_array<Component>::iterator sparse_array<Component>::end()
{
    return _data.end();
}

/**
         * @brief get the constant end iterator of the sparse array
         * 
         * @returns constant end iterator of the sparse_array
         */
template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::end() const
{
    return _data.end();
}

/**
         * @brief get the constant end iterator of the sparse array
         * 
         * @returns constant end iterator of the sparse_array
         */
template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::cend() const
{
    return _data.cend();
}

/**
         * @brief resize the sparse array with a new size
         * 
         * @param new_size new wanted size of the sparse_array
         */
template <typename Component>
void sparse_array<Component>::resize(size_type new_size) {
    _data.resize(new_size);
}

/**
         * @brief get the size of the sparse_array
         * 
         * @return size of the array
         */
template<class Component>
inline typename sparse_array<Component>::size_type sparse_array<Component>::size() const
{
    return _data.size();
}

/**
         * @brief erase a specific index of the sparse array
         * 
         */
template<class Component>
inline void sparse_array<Component>::erase(size_type pos)
{
    if (pos < _data.size()) {
        _data[pos].reset();
    }
}

/**
         * @brief Get the index of a value
         * 
         * @return size_type 
         */
template<class Component>
inline typename sparse_array<Component>::size_type sparse_array<Component>::get_index(sparse_array<Component>::value_type const &value) const
{
    for (size_type i = 0; i < _data.size(); ++i) {
        if (_data[i].has_value() && _data[i].value() == value)
            return i;
    }
    return static_cast<size_type>(-1);
}

#endif // SPARSEARRAY_TPP