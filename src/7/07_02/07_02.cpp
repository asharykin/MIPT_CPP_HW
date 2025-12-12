#include <cassert>
#include <cmath>
#include <sstream>
#include <exception>
#include <variant>
#include <vector>

#include "rational.cpp"

bool equals(double x, double y, double epsilon = 0.00001)
{
    return std::abs(x - y) < epsilon;
}

void catch_exceptions()
{
    int* ptr = nullptr;
    try
    {
        size_t huge_size = 18'000'000'000'000'000'000;
        ptr = new int[huge_size]; // Trying to allocate a huge amount of memory
    }
    catch (const std::bad_alloc& exception)
    {
        std::cerr << "bad alloc: " << exception.what() << std::endl;
        // std::bad_alloc is thrown when memory allocation fails due to insufficient memory
        delete[] ptr;
    }

    try
    {
        std::variant<int, double> var = 5.0;
        std::cout << std::get<int>(var) << std::endl;
    }
    catch (const std::bad_variant_access& exception)
    {
        std::cerr << "bad variant access: " << exception.what() << std::endl;
        // std::bad_variant_access is thrown when accessing a variant with the wrong type
    }

    try
    {
        std::optional<int> opt; // No value is initialized
        std::cout << opt.value() << std::endl;
    }
    catch (const std::bad_optional_access& exception)
    {
        std::cerr << "bad optional access: " << exception.what() << std::endl;
        // std::bad_optional_access is thrown when you try to get the value from an empty optional
    }

    try
    {
        std::vector<int> vec;
        // Trying to create a vector with the size bigger than its maximum size
        vec.resize(vec.max_size() + 1);
    }
    catch (const std::length_error& exception)
    {
        std::cerr << "length error: " << exception.what() << std::endl;
        // std::length_error is thrown when you are trying to create a container with the size that is bigger than its maximum size
    }

    try
    {
        std::vector<int> vec = {1, 2, 3};
        std::cout << vec.at(5);
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "out of range: " << e.what() << std::endl;
        // std::out_of_range is thrown when you are trying to access an element outside of the container's bounds
    }

}

int main()
{
    Rational x(1); // 1/1
    Rational y(2, 1); // 2/1

    assert(equals(static_cast<double>(x), 1.0));

    assert((x += y) == Rational(3, 1));
    assert((x -= y) == Rational(1, 1));
    assert((x *= y) == Rational(2, 1));
    assert((x /= y) == Rational(1, 1));

    assert((x++) == Rational(1, 1));
    assert((x--) == Rational(2, 1));
    assert((++y) == Rational(3, 1));
    assert((--y) == Rational(2, 1));

    assert((x + y) == Rational(3, 1));
    assert((x - y) == Rational(-1, 1));
    assert((x * y) == Rational(2, 1));
    assert((x / y) == Rational(1, 2));

    assert((x += 1) == Rational(2, 1));
    assert((x + 1) == Rational(3, 1));
    assert((1 + y) == Rational(3, 1));
    assert((1 + 1) == Rational(2, 1));

    assert((x < y) == 0);
    assert((x > y) == 0);
    assert((x <= y) == 1);
    assert((x >= y) == 1);
    assert((x == y) == 1);
    assert((x != y) == 0);

    std::stringstream stream_1("1/2");
    std::stringstream stream_2;
    stream_1 >> x;
    stream_2 << x;
    assert(stream_2.str() == stream_1.str());

    try {
        Rational q = Rational(3, 0);
    }
    catch (std::exception const& exception)
    {
        std::cerr << "main: " << exception.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "main: unknown exception" << std::endl;
    }

    catch_exceptions();
}
