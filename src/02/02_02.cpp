#include <iostream>
#include <cmath>

int main()
{
    const double eps = 0.000001;
    double a, b, c;

    std::cout << "Enter coefficients a, b, and c: " << std::endl;
    std::cin >> a >> b >> c;

    if (std::abs(a) < eps)
    {
        if (std::abs(b) < eps)
        {
            if (std::abs(c) < eps)
            {
                std::cout << "The equation has infinitely many roots" << std::endl;
            }
            else
            {
                std::cout << "The equation has no roots" << std::endl;
            }
        }
        else
        {
            double root = -c / b;
            std::cout << "The equation has one root: x = " << root << std::endl;
        }
    }
    else
    {
        double D = b * b - 4 * a * c;

        if (std::abs(D) < eps)
        {
            double root = -b / (2 * a);
            std::cout << "The equation has one root: x = " << root << std::endl;
        }
        else if (D >= eps)
        {
            double root1 = (-b + std::sqrt(D)) / (2 * a);
            double root2 = (-b - std::sqrt(D)) / (2 * a);
            std::cout << "The equation has two roots: x1 = " << root1 << ", x2 = " << root2 << std::endl;
        }
        else
        {
            std::cout << "The equation has no real roots" << std::endl;
        }
    }
}
