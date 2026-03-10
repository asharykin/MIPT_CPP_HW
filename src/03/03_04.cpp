#include <cassert>

class Entity
{
private:
    static int test_v1()
    {
        return 1;
    }

    static int test_v2()
    {
        return 2;
    }

    friend class Attorney_v1;
    friend class Attorney_v2;
};

class Attorney_v1
{
private:
    static int test_v1()
    {
        return Entity::test_v1();
    }

    friend class Tester_v1;
};

class Attorney_v2
{
private:
    static int test_v2()
    {
        return Entity::test_v2();
    }

    friend class Tester_v2;
};

class Tester_v1
{
public:
    static int test_v1()
    {
        return Attorney_v1::test_v1();
    }
};

class Tester_v2
{
public:
    static int test_v2()
    {
        return Attorney_v2::test_v2();
    }
};

int main()
{
    assert(Tester_v1::test_v1() == 1);
    assert(Tester_v2::test_v2() == 2);
}
