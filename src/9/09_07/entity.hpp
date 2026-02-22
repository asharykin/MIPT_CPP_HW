#pragma once
//#include <boost/noncopyable.hpp>
#include <array>
#include <cstddef>

class Entity //: private boost::noncopyable
{
public:
    Entity();

    Entity(Entity && other) noexcept;

    ~Entity();

    Entity & operator=(Entity&& other) noexcept;

    void test() const;

private:

    class Implementation;

    Implementation* get();

    const Implementation* get() const;

    alignas(std::max_align_t) std::array<std::byte, 16> m_storage;
};