#include <array>
#include <cmath>

consteval double compute_e(double eps)
{
    double m = 1.0;
    double e = m;
    int i = 1;

    while (m >= eps) {
        m /= i;
        e += m;
        ++i;
    }
    return e;
}

consteval double compute_pi(double eps)
{
    double m = 1.0;
    double pi = m;
    int i = 1;

    while (std::fabs(m) >= eps / 4) {
        m = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        pi += m;
        ++i;
    }
    return 4 * pi;
}

int main()
{
    constexpr std::array epsilons = {0.01, 0.0001, 0.00001};

    constexpr double e1 = compute_e(epsilons[0]);
    constexpr double e2 = compute_e(epsilons[1]);
    constexpr double e3 = compute_e(epsilons[2]);

    static_assert(e1 > 2.7 && e1 < 2.8);
    static_assert(e2 > 2.718 && e2 < 2.719);
    static_assert(e3 > 2.7182 && e3 < 2.7183);

    constexpr double pi1 = compute_pi(epsilons[0]);
    constexpr double pi2 = compute_pi(epsilons[1]);
    constexpr double pi3 = compute_pi(epsilons[2]);

    static_assert(pi1 > 3.1 && pi1 < 3.2);
    static_assert(pi2 > 3.141 && pi2 < 3.142);
    static_assert(pi3 > 3.1415 && pi3 < 3.1416);
}
