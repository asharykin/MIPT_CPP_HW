#include "entity.hpp"
#include <utility>
#include <memory>
#include <bit>
#include <iostream>

class Entity::Implementation
{
public:
    void test() const
    {
        std::cout << "Entity::Implementation::test const\n";
    }
};

Entity::Implementation* Entity::get()
{
    std::cout << "Entity::Implementation::get\n";
    auto* ptr = std::bit_cast<Implementation*>(m_storage.data());
    return std::launder(ptr);
}

const Entity::Implementation* Entity::get() const
{
    std::cout << "Entity::Implementation::get const\n";
    auto* ptr = std::bit_cast<const Implementation*>(m_storage.data());
    return std::launder(ptr);
}

Entity::Entity()
{
    static_assert(sizeof(Implementation) <= sizeof(m_storage));
    static_assert(alignof(Implementation) <= alignof(std::max_align_t));
    std::cout << "Entity::Entity()\n";
    new (m_storage.data()) Implementation();
}

Entity::Entity(Entity && other) noexcept
{
    std::cout << "Entity::Entity(Entity &&)\n";
    new (m_storage.data()) Implementation(std::move(*other.get()));
}

Entity::~Entity()
{
    std::cout << "Entity::~Entity()\n";
    std::destroy_at(get());
}

Entity & Entity::operator=(Entity && other) noexcept
{
    std::cout << "Entity::operator=(Entity &&)\n";
    if (this != &other)
    {
        std::destroy_at(get());
        new (m_storage.data()) Implementation(std::move(*other.get()));
    }
    return *this;
}

void Entity::test() const
{
    get()->test();
}