#include <cassert>
#include <iostream>

class Entity
{
public :

    virtual ~Entity() = default;

    virtual int test() const = 0;
};

class Client : public virtual Entity
{
public :

    int test() const override
    {
        return 1;
    }
};

class Server : public virtual Entity
{
public :

    int test() const override
    {
        return 2;
    }
};

template<typename T> class Decorator : public virtual Entity, private T
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
    Entity* c = new Decorator<Client>();
    Entity* s = new Decorator<Server>();
    assert(c->test() == 11);
    assert(s->test() == 12);
    delete c;
    delete s;
}
