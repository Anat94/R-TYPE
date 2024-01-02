/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** SparseArray
*/

#ifndef SPARSEARRAY_TPP
    #define SPARSEARRAY_TPP
    #pragma warning(disable: 4668)
    #pragma warning(disable: 4626)
    #pragma warning(disable: 4625)
    #pragma warning(disable: 4820)
#pragma warning(disable: 5031)
#pragma warning(disable: 4365)
#pragma warning(disable: 5027)
#pragma warning(disable: 4514)
#pragma warning(disable: 4464)
#pragma warning(disable: 5026)
#pragma warning(disable: 4457)
#pragma warning(disable: 5262)
#pragma warning(disable: 5204)
#pragma warning(disable: 4355)
#pragma warning(disable: 5220)
#pragma warning(disable: 5039)

template <typename Component>
inline sparse_array<Component>::sparse_array(const sparse_array &other) : _data(other._data) {}

template <typename Component>
inline sparse_array<Component>::sparse_array(sparse_array &&other) noexcept : _data(std::move(other._data)) {}

template <typename Component>
inline sparse_array<Component> &sparse_array<Component>::operator=(const sparse_array<Component> &other)
{
    if (this != &other)
        _data = other._data;
    return *this;
}

template <typename Component>
inline sparse_array<Component> &sparse_array<Component>::operator=(sparse_array<Component> &&other) noexcept
{
    if (this != &other)
        _data = std::move(other._data);
    return *this;
}

template<class Component>
inline typename sparse_array<Component>::reference_type sparse_array<Component>::operator[](size_t idx)
{
    if (idx >= _data.size()) {
        _data.resize(idx + 1);
    }
    // if (!_data[idx] || !_data[idx].has_value())
    //     throw std::out_of_range("😉: out of range for []");
    return _data[idx];
}

template <typename Component>
inline typename sparse_array<Component>::const_reference_type sparse_array<Component>::operator[](size_t idx) const
{
    if (idx < _data.size() && _data[idx])
        return _data[idx];
    else
        throw std::out_of_range("🙂: out of range for [] but const");
}

template<class Component>
inline typename sparse_array<Component>::iterator sparse_array<Component>::begin()
{
    return _data.begin();
}

template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::begin() const
{
    return _data.begin();
}

template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::cbegin() const
{
    return _data.cbegin();
}

template<class Component>
inline typename sparse_array<Component>::iterator sparse_array<Component>::end()
{
    return _data.end();
}

template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::end() const
{
    return _data.end();
}

template<class Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::cend() const
{
    return _data.cend();
}

template<class Component>
inline typename sparse_array<Component>::size_type sparse_array<Component>::size() const
{
    return _data.size();
}

// template<class Component>
// inline typename sparse_array<Component>::reference_type &sparse_array<Component>::insert_at(size_type pos, const Component &value)
// {
//     if (pos >= _data.size())
//         _data.resize(pos + 1);
//     _data[pos] = value;
//     return _data[pos];
// }

// template<class Component>
// inline typename sparse_array<Component>::reference_type &sparse_array<Component>::insert_at(size_type pos, Component &&value)
// {
//     if (pos >= _data.size())
//         _data.resize(pos + 1);
//     _data[pos] = std::move(value);
//     return _data[pos];
// }

template<class Component>
inline void sparse_array<Component>::erase(size_type pos)
{
    if (pos < _data.size()) {
        _data[pos].reset();
    } else
        throw std::out_of_range("🤥: out of range for erase");
}

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