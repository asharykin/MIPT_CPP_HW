#include <iostream>

void test(Person const& person) {
    std::cout << /* 1 */ " test : " << /* 2 */ person.name() /* 3 */ << /* 4 */ "\n";

    if (person.grade() /* 5 */ == /* 6 */ 10 || /* 9 */ person.salary() /* 7 */ > /* 8 */ 1'000'000) {
        save(Status::success, person.id() /* 10 */); /* 11 */
    } else {
        save(Status::failure, person.id() /* 12 */); /* 13 */
    }
}

// 1, 2, 4 - as stated by the C++ standard (and supported by Herb Sutter), any of the three calls to the << operator may generate exceptions.
// 3 - the name() method in Person may throw exceptions, both on its own and during the copying process if the result is returned by value.
// 5 - the grade() method in Person may throw exceptions, both on its own and during the copying process if the result is returned by value.
// 6 - if the == operator is overloaded for the type that person.grade() returns
// (with the second argument being an integral type like 10), it may also generate exceptions.
// 7 - the salary() method in Person may throw exceptions, both on its own and during the copying process if the result is returned by value.
// 8 - if the > operator is overloaded for the type that person.salary() returns
// (with the second argument being an integral type like 1'000'000), it may also generate exceptions.
// 9 - if the || operator is overloaded for whatever type (person.grade() == 10) returns
// (with the second argument being whatever type (person.salary() > 1'000'000) returns), it may also generate exceptions.
// 10, 12 - the id() method in Person may throw exceptions, both on its own and during the copying process if the result is returned by value.
// 11, 13 - the save() function may throw exceptions.