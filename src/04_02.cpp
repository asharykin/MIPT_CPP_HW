#include <iostream>

template<typename T>
T max(T val)
{
    return val;
}

template<typename T, typename ... Ts>
T max(T first, Ts... rest)
{
    return first > max(rest...) ? first : max(rest...);
}

template<typename T>
T min(T val)
{
    return val;
}

template<typename T, typename ... Ts>
T min(T first, Ts ... rest)
{
    return first < min(rest...) ? first : min(rest...);
}

template<typename ... Ts>
double sum(Ts ... rest)
{
    return (rest + ...);
}

template<typename ... Ts>
double average(Ts ... rest)
{
    return sum(rest...) / sizeof...(rest);
}

int main() {
    std::cout << "max: " << max(3.5, 7.2, 1.5, 4.8) << std::endl;
    std::cout << "min: " << min(3.5, 7.2, 1.5, 4.8) << std::endl;
    std::cout << "sum: " << sum(3.5, 7.2, 1.5, 4.8) << std::endl;
    std::cout << "average: " << average(3.5, 7.2, 1.5, 4.8) << std::endl;
}
