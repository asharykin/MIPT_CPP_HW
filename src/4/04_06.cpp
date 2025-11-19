#include <array>

consteval double compute_e(double eps)
{
    double m = 1.0;
    double e = 0.0;
    for (int i = 1; m >= eps; ++i)
    {
        e += m;
        m /= i;
    }
    return e;
}

int main()
{
    constexpr std::array epsilons = {0.01, 0.001, 0.0001};

    constexpr double e1 = compute_e(epsilons[0]);
    constexpr double e2 = compute_e(epsilons[1]);
    constexpr double e3 = compute_e(epsilons[2]);

    static_assert(e1 > 2.7 && e1 < 2.8);
    static_assert(e2 > 2.71 && e2 < 2.72);
    static_assert(e3 > 2.718 && e3 < 2.719);
}