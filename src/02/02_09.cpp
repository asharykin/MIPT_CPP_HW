#include <cassert>
#include <numeric>

int gcd_recursive(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return gcd_recursive(b, a % b);
}

int gcd_iterative(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b)
{
    return a / gcd_iterative(a, b) * b; // or a / gcd_recursive(a, b) * b
}

int main()
{
    int a = 12, b = 8;
    assert(std::gcd(a, b) == 4);
    assert(gcd_recursive(a, b) == 4);
    assert(gcd_iterative(a, b) == 4);
    assert(std::lcm(a, b) == 24);
    assert(lcm(a, b) == 24);
}
