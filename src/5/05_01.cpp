#include <iostream>
#include <string>
#include <cassert>

class Person
{
public:
    std::string name {};
    int age = 0;
    int grade = 0;
};

class Builder
{
public:
    Builder() : m_person(new Person()) { };

    Builder& name(std::string const& name)
    {
        m_person->name = name;
        return *this;
    }

    Builder& age(int age)
    {
        m_person->age = age;
        return *this;
    }

    Builder& grade(int grade)
    {
        m_person->grade = grade;
        return *this;
    }

    Person* get()
    {
        return m_person;
    }

private:
    Person* m_person = nullptr;
};

int main()
{
    Builder builder;
    auto person = builder.name("Ivan").age(25).grade(10).get();

    assert(person->name == "Ivan");
    assert(person->age == 25);
    assert(person->grade == 10);

    delete person;
}
