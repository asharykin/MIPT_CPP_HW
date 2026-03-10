#include <assert.h>

class Entity_v1
{
public:
    virtual ~Entity_v1() = default;

    virtual int test()
    {
        return 1;
    }
};

class Entity_v2
{
public:
    virtual ~Entity_v2() = default;

    virtual int test()
    {
        return 2;
    }
};

class Adapter_v1 : public Entity_v1
{
protected:
    virtual int test_v1()
    {
        return 11;
    }

public:
    ~Adapter_v1() = default;

    int test() override
    {
        return test_v1();
    }
};

class Adapter_v2 : public Entity_v2
{
protected:
    virtual int test_v2()
    {
        return 22;
    }

public:
    ~Adapter_v2() = default;

    int test() override
    {
        return test_v2();
    }
};


class Client : public Adapter_v1, public Adapter_v2
{
public:
    ~Client() = default;

    int test_v1() override
    {
        return Adapter_v1::test_v1() + 1;
    }

    int test_v2() override
    {
        return Adapter_v2::test_v2() + 2;
    }
};

int main()
{
    Client client;
    Entity_v1* ev1 = &client;
    Entity_v2* ev2 = &client;
    assert(ev1->test() == 12);
    assert(ev2->test() == 24);
}
