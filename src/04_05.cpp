#include <iostream>
#include <limits>

template<int A, int B> struct CheckOverflow
{
    static inline bool const value = A <= std::numeric_limits<int>::max() - B;
};

template<int N> struct Fibonacci
{
    static_assert(N >= 0, "N must be non-negative");

    static_assert(CheckOverflow<Fibonacci<N - 1>::value, Fibonacci<N - 2>::value>::value,
                  "Integer overflow in Fibonacci calculation");

    static inline int const value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<> struct Fibonacci<0> { static inline int const value = 0; };

template<> struct Fibonacci<1> { static inline int const value = 1; };

template<int N> int const FibonacciValue = Fibonacci<N>::value;


int main() {
    static_assert(FibonacciValue<2> == 1, "Fibonacci(2) should be 1");
    static_assert(FibonacciValue<3> == 2, "Fibonacci(3) should be 2");
    static_assert(FibonacciValue<4> == 3, "Fibonacci(4) should be 3");
    static_assert(FibonacciValue<5> == 5, "Fibonacci(5) should be 5");
    static_assert(FibonacciValue<6> == 8, "Fibonacci(6) should be 8");
}
