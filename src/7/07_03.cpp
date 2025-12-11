#include <iostream>

void test (Person const& person )
{
    std::cout << " test : " << person.name() << "\n";
    if (person.grade() == 10 || person.salary () > 1000000)
    {
        save(Status::success, person.id()) ;
    }
    else
    {
        save(Status::failure, person.id()) ;
    }
}
