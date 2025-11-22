#include <iostream>
#include <cmath>

double compute_e(double eps)
{
    double m = 1.0;
    double e = m;
    int i = 1;

    while (m >= eps)
    {
        m /= i;
        e += m;
        ++i;
    }
    return e;
}

double compute_pi(double eps)
{
    double m = 1.0;
    double pi = m;
    int i = 1;

    while (std::fabs(m) >= eps / 4)
    {
        m = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
        pi += m;
        ++i;
    }
    return 4 * pi;
}

int main()
{
    double eps;
    std::cout << "Enter epsilon: " << std::endl;
    std::cin >> eps;

    double e = compute_e(eps);
    double pi = compute_pi(eps);
    std::cout << "e: " << e << std::endl;
    std::cout << "pi: " << pi << std::endl;
}
