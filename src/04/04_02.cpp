#include <cassert>
#include <iostream>

template<typename T> T max(T val)
{
    return val;
}

template<typename T, typename ... Ts> T max(T first, Ts ... rest)
{
    return first > max(rest...) ? first : max(rest...);
}

template<typename T> T min(T val)
{
    return val;
}

template<typename T, typename ... Ts> T min(T first, Ts ... rest)
{
    return first < min(rest...) ? first : min(rest...);
}

template<typename ... Ts> double sum(Ts ... rest)
{
    return (rest + ...);
}

template<typename ... Ts> double average(Ts ... rest)
{
    return sum(rest...) / sizeof...(rest);
}

bool equal(double x, double y, double epsilon = 0.00001)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    assert(equal(max(1.1, 2.2, 3.3, 4.4), 4.4));
    assert(equal(min(1.1, 2.2, 3.3, 4.4), 1.1));
    assert(equal(sum(1.1, 2.2, 3.3, 4.4), 11));
    assert(equal(average(1.1, 2.2, 3.3, 4.4), 2.75));
}
