#include "entity.hpp"
#include <iostream>
#include <utility>

int main()
{
    static_assert(sizeof(Entity) == 16);

    Entity e1;
    e1.test();

    std::cout << std::endl;

    Entity e2 = std::move(e1);
    e2.test();

    std::cout << std::endl;

    Entity e3;
    e3 = std::move(e2);
    e3.test();

    std::cout << std::endl;

    const Entity e_const;
    e_const.test();

    std::cout << std::endl;
}
