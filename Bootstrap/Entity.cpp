#include <any>
#include <iostream>

class Entity {
public:
    Entity(std::any value) : _value(value) {}
    ~Entity() = default;
    size_t getValue() const;

private:
    std::any _value;
};

size_t Entity::getValue() const {
    return static_cast<size_t>(std::any_cast<int>(_value));
}

int main() {
    Entity tmp(10);
    std::cout << "Entity " << tmp.getValue() << std::endl;
    return 0;
}