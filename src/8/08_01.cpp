#include <iostream>

// Класс с закрытыми данными
class Entity_v1
{
public:
    Entity_v1(int value) : privateValue(value) { }

    void printValue() const
    {
        std::cout << "Entity_v1.privateValue = " << privateValue << std::endl;
    }

private:
    int privateValue;
};

class Entity_v2
{
public:
    int publicValue;
};

int main()
{
    Entity_v1 e1(42);
    e1.printValue();

    reinterpret_cast<Entity_v2&>(e1).publicValue = 100;

    e1.printValue();
}
