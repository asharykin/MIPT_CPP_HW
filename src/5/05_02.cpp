#include <cassert>
#include <iostream>

class Entity
{
public :

    virtual ~Entity() = default;

    virtual int test() const = 0;
};

class Client : public Entity
{
public :

    int test() const override
    {
        return 1;
    }
};

class Server : public Entity
{
public :

    int test() const override
    {
        return 2;
    }
};

template<typename T> class Decorator : public T
{
public :

    int test() const override
    {
        int test = T::test();
        return 10 + test;
    }
};

int main()
{
    Decorator<Client> c;
    Decorator<Server> s;
    assert(c.test() == 11);
    assert(s.test() == 12);
}
