#include <typeindex>
#include <unordered_map>
#include <any>
#include <vector>

class Entity {
    private:
        size_t data;

    explicit Entity(std::size_t value) : data(value) {}

    operator std::size_t() const {
        return data;
    }
};

template <typename Component>
class sparse_array {
    public:
        using value_type = ???;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector <value_type>;
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;
    public:
        sparse_array(); // You can add more constructors .
        sparse_array(sparse_array const &); // copy constructor
        sparse_array(sparse_array &&) noexcept; // move constructor
        ~sparse_array();

        sparse_array &operator=(sparse_array const &); // copy assignment operator
        sparse_array &operator=(sparse_array &&) noexcept ; // move assignment operator

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
        template <class ... Params>
        reference_type emplace_at(size_type pos, Params &&...); // optional
        void erase(size_type pos);

        size_type get_index(value_type const &)const;
    private:
        container_t _data ;
};

template <typename Component>
typename sparse_array<Component>::reference_type sparse_array<Component>::insert_at(size_type pos, const Component &value) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = value;
    return _data[pos];
}

template <typename Component>
typename sparse_array<Component>::reference_type sparse_array<Component>::insert_at(size_type pos, Component &&value) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos] = std::move(value);
    return _data[pos];
}

template <typename Component>
template <class... Params>
typename sparse_array<Component>::reference_type sparse_array<Component>::emplace_at(size_type pos, Params &&... args) {
    if (pos >= _data.size()) {
        _data.resize(pos + 1);
    }
    _data[pos].~Component();
    new (&_data[pos]) Component(std::forward<Params>(args)...);
    return _data[pos];
}

template <typename Component>
void sparse_array<Component>::erase(size_type pos) {
    if (pos < _data.size()) {
        _data[pos].~Component();
        _data.erase(_data.begin() + static_cast<typename container_t::difference_type>(pos));
    }
}

template <typename Component>
typename sparse_array<Component>::size_type sparse_array<Component>::get_index(const value_type &value) const {
    for (size_type i = 0; i < _data.size(); ++i) {
        if (_data[i] == value) {
            return i;
        }
    }
    return static_cast<size_type>(-1);
}

class registry {
    public :
        template <class Component>
        sparse_array<Component>& register_component();

        template <class Component>
        sparse_array<Component>& get_components();

        template <class Component>
        const sparse_array<Component>& get_components() const;

        entity_manager::entity_t spawn_entity();
        entity_manager::entity_t entity_from_index(std::size_t idx);
        void kill_entity(entity_manager::entity_t const& e);

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_manager::entity_t const& to, Component&& c);

        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_manager::entity_t const& to, Params&&... p);

        template <typename Component>
        void remove_component(entity_manager::entity_t const& from);

    private :
        entity_manager _entity_manager;
        using type_index_t = std::type_index;
        using component_map_t = std::unordered_map<type_index_t, std::any>;

        template <class Component>
        sparse_array<Component>& create_or_get_array();

        component_map_t _components_arrays;
};

template <class Component>
sparse_array<Component>& registry::register_component() {
    return create_or_get_array<Component>();
}

template <class Component>
sparse_array<Component>& registry::get_components() {
    return create_or_get_array<Component>();
}

template <class Component>
const sparse_array<Component>& registry::get_components() const {
    return create_or_get_array<Component>();
}

template <class Component>
sparse_array<Component>& registry::create_or_get_array() {
    type_index_t typeIndex = typeid(Component);

    auto it = _components_arrays.find(typeIndex);
    if (it == _components_arrays.end()) {
        auto [newIt, success] = _components_arrays.emplace(typeIndex, sparse_array<Component>());
        return std::any_cast<sparse_array<Component>&>(newIt->second);
    } else {
        return std::any_cast<sparse_array<Component>&>(it->second);
    }
};

class entity_manager {
    public:
        using entity_t = std::size_t;

        entity_t spawn_entity();
        entity_t entity_from_index(std::size_t idx);
        void kill_entity(entity_t const& e);

    private:
        std::vector<bool> _alive_entities;
        std::vector<entity_t> _free_indices;
};

entity_manager::entity_t entity_manager::spawn_entity() {
    entity_t new_entity;
    if (_free_indices.empty()) {
        new_entity = static_cast<entity_t>(_alive_entities.size());
        _alive_entities.push_back(true);
    } else {
        new_entity = _free_indices.back();
        _free_indices.pop_back();
        _alive_entities[new_entity] = true;
    }
    return new_entity;
}

entity_manager::entity_t entity_manager::entity_from_index(std::size_t idx) {
    if (idx < _alive_entities.size() && _alive_entities[idx]) {
        return static_cast<entity_t>(idx);
    } else {
        return static_cast<entity_t>(-1);
    }
}

void entity_manager::kill_entity(entity_manager::entity_t const& e) {
    if (e < _alive_entities.size() && _alive_entities[e]) {
        _alive_entities[e] = false;
        _free_indices.push_back(e);
    }
}

template <class Component>
sparse_array<Component>& registry::register_component() {
    return create_or_get_array<Component>();
}

template <class Component>
sparse_array<Component>& registry::get_components() {
    return create_or_get_array<Component>();
}

template <class Component>
const sparse_array<Component>& registry::get_components() const {
    return create_or_get_array<Component>();
}

entity_manager::entity_t registry::spawn_entity() {
    return _entity_manager.spawn_entity();
}

entity_manager::entity_t registry::entity_from_index(std::size_t idx) {
    return _entity_manager.entity_from_index(idx);
}

void registry::kill_entity(entity_manager::entity_t const& e) {
    _entity_manager.kill_entity(e);

    // Remove all components associated with the entity
    for (auto& [typeIndex, componentArray] : _components_arrays) {
        componentArray.erase(e);
    }
}

template <typename Component>
typename sparse_array<Component>::reference_type registry::add_component(entity_manager::entity_t const& to, Component&& c) {
    return get_components<Component>().insert_at(to, std::forward<Component>(c));
}

template <typename Component, typename... Params>
typename sparse_array<Component>::reference_type registry::emplace_component(entity_manager::entity_t const& to, Params&&... p) {
    return get_components<Component>().emplace_at(to, std::forward<Params>(p)...);
}

template <typename Component>
void registry::remove_component(entity_manager::entity_t const& from) {
    get_components<Component>().erase(from);
}

template <class Component>
sparse_array<Component>& registry::create_or_get_array() {
    type_index_t typeIndex = typeid(Component);

    auto it = _components_arrays.find(typeIndex);
    if (it == _components_arrays.end()) {
        // If the array does not exist, create and store a new one
        auto [newIt, success] = _components_arrays.emplace(typeIndex, sparse_array<Component>());
        return std::any_cast<sparse_array<Component>&>(newIt->second);
    } else {
        // If the array already exists, return it
        return std::any_cast<sparse_array<Component>&>(it->second);
    }
}

