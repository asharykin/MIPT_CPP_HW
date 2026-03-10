#include <iostream>
#include <cmath>
#include <cassert>
#include <numbers>

class Triangle
{
public:
    Triangle(double a, double b, double c) : m_a(a), m_b(b), m_c(c)
    {
        if (m_a <= 0.0 || m_b <= 0.0 || m_c <= 0.0 ||
            (m_a + m_b <= m_c) || (m_a + m_c <= m_b) || (m_b + m_c <= m_a))
        {
            m_a = m_b = m_c = 0.0;
        }
    }

    double perimeter() const
    {
        return m_a + m_b + m_c;
    }

    double area() const
    {
        double s = perimeter() / 2;
        return std::sqrt(s * (s - m_a) * (s - m_b) * (s - m_c)); // Heron's formula
    }

private:
    double m_a;
    double m_b;
    double m_c;
};

class Square
{
public:
    Square(double side) : m_side(side > 0.0 ? side : 0.0) {}

    double perimeter() const
    {
        return 4 * m_side;
    }

    double area() const
    {
        return m_side * m_side;
    }

private:
    double m_side;
};

class Circle
{
public:
    Circle(double radius) : m_radius(radius > 0.0 ? radius : 0.0) {}

    double perimeter() const
    {
        return 2 * std::numbers::pi * m_radius;
    }

    double area() const
    {
        return std::numbers::pi * m_radius * m_radius;
    }

private:
    double m_radius;
};

bool equal(double x, double y, double epsilon = 0.000001)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    Triangle triangle(3.0, 4.0, 5.0);
    assert(equal(triangle.perimeter(), 12.0));
    assert(equal(triangle.area(), 6.0));

    Square square(2.0);
    assert(equal(square.perimeter(), 8.0));
    assert(equal(square.area(), 4.0));

    Circle circle(1.0);
    assert(equal(circle.perimeter(), 2 * std::numbers::pi));
    assert(equal(circle.area(), std::numbers::pi));
}