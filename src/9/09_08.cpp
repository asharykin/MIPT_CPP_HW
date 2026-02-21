#include <cstddef>
#include <iostream>
#include <new>

///////////////////////////////////////////////////////////////////

template < typename D > class Entity
{
public :

    static auto operator new(std::size_t size) -> void *
    {
        std::cout << std::format("Entity::operator new\n");

        return ::operator new(size);
    }

    static void operator delete(void * x) noexcept
    {
        std::cout << std::format("Entity::operator delete\n");

        ::operator delete(x);
    }

    //  ----------------------------------------------------

    static auto operator new[](std::size_t size) -> void *
    {
        std::cout << std::format("Entity::operator new[]\n");

        return ::operator new[](size);
    }

    static void operator delete[](void * x) noexcept
    {
        std::cout << std::format("Entity::operator delete[]\n");

        ::operator delete[](x);
    }

    //  ----------------------------------------------------

    static auto operator new(std::size_t size, const std::nothrow_t & nt) noexcept -> void *
    {
        std::cout << std::format("Entity::operator new (nothrow)\n");

        return ::operator new(size, nt);
    }

    static void operator delete(void * x, const std::nothrow_t &) noexcept
    {
        std::cout << std::format("Entity::operator delete (nothrow)\n");

        ::operator delete(x);
    }

    //  ----------------------------------------------------

    static auto operator new[](std::size_t size, const std::nothrow_t & nt) noexcept -> void *
    {
        std::cout << std::format("Entity::operator new[] (nothrow)\n");

        return ::operator new[](size, nt);
    }

    static void operator delete[](void * x, const std::nothrow_t &) noexcept
    {
        std::cout << std::format("Entity::operator delete[] (nothrow)\n");

        ::operator delete[](x);
    }

protected :

    Entity() = default;

    ~Entity() = default;
};

///////////////////////////////////////////////////////////////////

class Client : private Entity < Client >
{
private :

    using base_t = Entity < Client > ;

public :

    Client() { std::cout << std::format("Client::Client\n"); }

    ~Client() { std::cout << std::format("Client::~Client\n"); }

    //  ----------------------------------------------------

    using base_t::operator new;

    using base_t::operator delete;

    using base_t::operator new[];
    
    using base_t::operator delete[];
};

///////////////////////////////////////////////////////////////////

int main()
{
    delete new Client;

    delete[] new Client[2];

    Client* p1 = new (std::nothrow) Client;
    operator delete(p1, std::nothrow);

    Client* p2 = new (std::nothrow) Client[3];
    operator delete[](p2, std::nothrow);
}

///////////////////////////////////////////////////////////////////