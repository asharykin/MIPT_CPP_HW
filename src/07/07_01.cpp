#include <iostream>
#include <cmath>
#include <optional>
#include <variant>
#include <cassert>

using Roots = std::variant<std::monostate, double, std::pair<double, double>>;

bool equals(double x, double y, double epsilon = 0.00001)
{
    return std::abs(x - y) < epsilon;
}

bool greater_than(double x, double y, double epsilon = 0.00001) {
    return x - y > epsilon;
}

std::optional<Roots> solve(double a, double b, double c)
{
    if (equals(a, 0.0))
    {
        if (equals(b, 0.0))
        {
            if (equals(c, 0.0))
            {
                return std::monostate {};
            }
            return std::nullopt;
        }
        double root = -c / b;
        return root;
    }

    double D = b * b - 4 * a * c;

    if (equals(D, 0.0))
    {
        double root = -b / (2 * a);
        return root;
    }

    if (greater_than(D, 0.0))
    {
        double root1 = (-b + std::sqrt(D)) / (2 * a);
        double root2 = (-b - std::sqrt(D)) / (2 * a);
        return std::make_pair(root1, root2);
    }

    return std::nullopt;
}

int main()
{
    // 0*x^2 + 0*x + 0 = 0; infinitely many roots
    assert(std::holds_alternative<std::monostate>( solve(0.0, 0.0, 0.0).value() ));
    // 0*x^2 + 0*x + 2 = 0; no roots
    assert(!solve(0.0, 0.0, 2.0).has_value());
    // 0*x^2 + 1*x - 2 = 0; x = 2
    assert(equals( std::get<double>( solve(0.0, 1.0, -2.0).value() ), 2.0 ));
    // 1*x^2 - 2*x + 3 = 0; no roots
    assert(!solve(1.0, -2.0, 3.0).has_value());
    // 1*x^2 - 2*x + 1 = 0; x = 1
    assert(equals( std::get<double>( solve(1.0, -2.0, 1.0).value() ), 1.0 ));
    // 1*x^2 - 2*x + 0 = 0; x1 = 2, x2 = 0
    double x1 = std::get< std::pair<double, double> >( solve(1.0, -2.0, 0.0).value() ).first;
    double x2 = std::get< std::pair<double, double> >( solve(1.0, -2.0, 0.0).value() ).second;
    assert(equals(x1, 2.0));
    assert(equals(x2, 0.0));
}
